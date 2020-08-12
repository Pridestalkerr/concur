#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <vector>
#include <queue>



namespace thr
{



class Async
{

private:

	std::vector <std::thread> m_workers;
	std::queue <std::function <void()>> m_jobs;

	std::mutex m_mtx;
	std::condition_variable m_cv;

	enum Status {RUNNING, EXIT} m_status;

public:

	Async(size_t = std::thread::hardware_concurrency());

	~Async();

	template <typename F, typename... Args>
	std::future <typename std::result_of <F(Args...)>::type>
	operator()(F &&function, Args&&... args);

private:

	void thread_loop();

};


Async::Async(size_t count)
	:
	m_status(RUNNING) 
{
	for(size_t itr = 0; itr < count; ++itr)
	{
		m_workers.emplace_back(&Async::thread_loop, this);
	}
};


Async::~Async()
{
	{
        std::scoped_lock lock(m_mtx);
        m_status = EXIT;
    }

    // wake up all the threads, force them to exit
    m_cv.notify_all();

    for(std::thread &worker : m_workers)
    {
    	worker.join();
    }
};


void Async::thread_loop()
{
	std::function <void()> job;
	std::unique_lock <std::mutex> lock(m_mtx, std::defer_lock);

RUN:

	lock.lock();
    m_cv.wait(lock, [this]{ return !m_jobs.empty() || m_status == EXIT; });

    if(m_status == EXIT) return;

    job = std::move(m_jobs.front());
    m_jobs.pop();

    job();

    lock.unlock();
    
    goto RUN;
};


template <typename F, typename... Args>
std::future <typename std::result_of <F(Args...)>::type>
Async::operator()(F &&function, Args&&... args)
{
	using result_type = typename std::result_of <F(Args...)>::type;

	auto context = std::make_shared <std::packaged_task <result_type()>>(
        std::bind(std::forward <F>(function), std::forward <Args>(args)...)
    );

	{
		// add job to the queue, synchronize access
		std::scoped_lock lock(m_mtx);
		m_jobs.emplace( [context](){ (*context)(); } );
	}

	// wake up a single thread waiting for a job
	m_cv.notify_one();

    return context->get_future();
};



}