#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <functional>



namespace thr
{



// A persistent thread...

class Thread
{
    
    std::condition_variable m_cv;
    std::mutex m_mtx;
    // -1 join thread 
    //  0 thread signals wait...
    //  1 signal to run
    int m_status;

    std::thread m_worker;

    std::function <void()> m_context;    // execution context

public:

    Thread();
    ~Thread();

    template <typename F, typename... Args>
    std::future <typename std::result_of <F(Args...)>::type> run(F&&, Args&&...);

    void wait();

    void worker();

};


Thread::Thread()
    :
    m_status(0),
    m_worker(&thr::Thread::worker, this)
{};

Thread::~Thread()
{
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_status = -1;
    }
    m_cv.notify_one();
    m_worker.join();
};


void Thread::worker()
{
RUN:

    std::unique_lock <std::mutex> lock(m_mtx);
    m_cv.wait(lock, [this]{ return m_status != 0; });

    if(m_status == -1) return;

    m_context();
    m_status = 0;

    lock.unlock();
    m_cv.notify_one();
    
    goto RUN;
}

template <typename F, typename... Args>
std::future <typename std::result_of <F(Args...)>::type>
Thread::run(F &&function, Args&&... args)
{
    using result_type = typename std::result_of <F(Args...)>::type;

    std::lock_guard<std::mutex> lock(m_mtx);
    m_status = 1;

    auto context =
        std::make_shared <std::packaged_task <result_type()>>(
            std::bind(std::forward <F>(function), std::forward <Args>(args)...)
        );

    // capture a pointer to the created execution context
    // this will make sure the function stays around
    m_context = [context](){ (*context)(); };

    m_cv.notify_one();

    return context->get_future();
};

void Thread::wait()
{
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cv.wait(lock, [this]{ return m_status == 0; });
};

//



}