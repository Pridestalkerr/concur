#pragma once

#include <future>
#include <functional>



namespace con
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

    Static_context(F&);
    Static_context(F&&);

    template <typename... Args_>
    std::future <R> set(Args_&&...);

    void execute();

};

template <typename R, typename... Args>
Static_context <R, Args...>::Static_context(F& function)
    :
    m_function(function)
{};

template <typename R, typename... Args>
Static_context <R, Args...>::Static_context(F&& function)
    :
    m_function(std::move(function))
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



}