namespace thr
{



template <typename Context_T>
class Persistent_thread
{
	Context_T m_context;

	std::condition_variable m_cv;
    std::mutex m_mtx;

    enum Status {STANDBY, RUNNING, EXIT} m_status;
    std::thread m_worker;

    void thread_loop();

public:

	Persistent_thread(Context_T&&);
	~Persistent_thread();

	template <typename... Args>
    auto run(Args&&...);

};


template <typename Context_T>
Persistent_thread <Context_T>::Persistent_thread(Context_T &&context)
	:
	m_context(std::forward <Context_T>(context)),
	m_status(STANDBY),
	m_worker(&Persistent_thread <Context_T>::thread_loop, this)
{};


template <typename Context_T>
Persistent_thread <Context_T>::~Persistent_thread()
{
	{
        std::unique_lock<std::mutex> lock(m_mtx);
        m_status = EXIT;
    }
    m_cv.notify_one();
    m_worker.join();
};


template <typename Context_T>
void Persistent_thread <Context_T>::thread_loop()
{
RUN:

    std::unique_lock <std::mutex> lock(m_mtx);
    m_cv.wait(lock, [this]{ return m_status != STANDBY; });

    if(m_status == EXIT) return;

    m_context.execute();
    m_status = STANDBY;

    lock.unlock();
    m_cv.notify_one();
    
    goto RUN;
};


template <typename Context_T>
template <typename... Args>
auto Persistent_thread <Context_T>::run(Args&&... args)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    auto future = m_context.set(std::forward <Args>(args)...);

    m_status = RUNNING;
    m_cv.notify_one();

    return std::move(future);
};



}