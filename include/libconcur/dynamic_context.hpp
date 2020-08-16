#pragma once

#include <future>
#include <functional>



namespace con
{



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