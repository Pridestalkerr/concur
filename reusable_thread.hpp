#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <functional>



namespace thr
{


	
template <typename R, typename... Args>
class Reusable_thread
{
	using F = std::function <R(Args...)>;

	F m_function;
    std::packaged_task <R()> m_runnable;
    std::future <R> m_ret;

	std::condition_variable m_cv;
    std::mutex m_mtx;

    enum Status {STANDBY, RUNNING, EXIT} m_status;
    std::thread m_worker;

    void thread_loop();

public:

	Reusable_thread(F);
	~Reusable_thread();

	template <typename... Args_>
	void run(Args_&&...);

	R wait();
};

template <typename R, typename... Args>
Reusable_thread <R, Args...>::Reusable_thread(F function)
	:
	m_status(STANDBY),
	m_function(std::forward <F> (function)),
	m_worker(&Reusable_thread <R, Args...>::thread_loop, this)
{};

template <typename R, typename... Args>
Reusable_thread <R, Args...>::~Reusable_thread()
{
	{
        std::unique_lock<std::mutex> lock(m_mtx);
        m_status = EXIT;
    }
    m_cv.notify_one();
    m_worker.join();
};

template <typename R, typename... Args>
void Reusable_thread <R, Args...>::thread_loop()
{
RUN:

    std::unique_lock <std::mutex> lock(m_mtx);
    m_cv.wait(lock, [this]{ return m_status != STANDBY; });

    if(m_status == EXIT) return;

    m_runnable();
    m_status = STANDBY;

    lock.unlock();
    m_cv.notify_one();
    
    goto RUN;
};

template <typename R, typename... Args>
template <typename... Args_>
void Reusable_thread <R, Args...>::run(Args_&&... args)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    m_runnable = std::packaged_task <R()> (
    	std::bind(m_function, std::forward <Args_>(args)...)
    );
    m_ret = m_runnable.get_future();

    m_status = RUNNING;
    m_cv.notify_one();
};

template <typename R, typename... Args>
R Reusable_thread <R, Args...>::wait()
{
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cv.wait(lock, [this]{ return m_status == STANDBY; });

    return m_ret.get();
};



}