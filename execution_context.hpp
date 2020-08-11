#include <future>
#include <functional>



namespace thr
{



// Only works with std::function, allowing the user to pass a lambda as function
// to be run would defeat the whole purpose of this class, that is having to
// construct the execution context once, and then only binding whenever we run.
// A lambda can be polymorphic, thus changing behaviour (function) depending on
// the arguments passed to run().

template <typename R, typename... Args>
class Static_context
{

	using F = std::function <R(Args...)>;

	F m_function;
    std::packaged_task <R()> m_context;

public:

	Static_context(F);

	template <typename... Args_>
	std::future <R> set(Args_&&...);

	void execute();

};


template <typename R, typename... Args>
Static_context <R, Args...>::Static_context(F function)
	:
	m_function(std::forward <F> (function))
{};


template <typename R, typename... Args>
template <typename... Args_>
std::future <R> Static_context <R, Args...>::set(Args_&&... args)
{
	m_context = std::packaged_task <R()> (
    	std::bind(m_function, std::forward <Args_>(args)...)
    );

    return m_context.get_future();
};

template <typename R, typename... Args>
void Static_context <R, Args...>::execute()
{
	m_context();
};


// Accepts both std::function and lambdas.

class Dynamic_context
{

	std::function <void()> m_context;

public:

	Dynamic_context() = default;

	template <typename F, typename... Args>
	std::future <typename std::result_of <F(Args...)>::type>
	set(F&&, Args&&...);

	void execute();

};


template <typename F, typename... Args>
std::future <typename std::result_of <F(Args...)>::type>
Dynamic_context::set(F &&function, Args&&... args)
{
	using result_type = typename std::result_of <F(Args...)>::type;

	auto context = std::make_shared <std::packaged_task <result_type()>>(
        std::bind(std::forward <F>(function), std::forward <Args>(args)...)
    );

    m_context = [context](){ (*context)(); };

    return context->get_future();
};

void Dynamic_context::execute()
{
	m_context();
};



}