#include <iostream>
#include "./include/thread.hpp"

int main()
{
    std::function <int(int, int)> f = [](int x, int y) -> int {
        std::cout << x << std::endl;
        return x * y;
    };

    thr::Thread t{ thr::Static_context(f) };
    auto ret = t.run(2, 2);
    std::cout << ret.get() << std::endl;

    thr::Thread t0{};
    ret = t0.run(f, 2, 2);
    std::cout << ret.get() << std::endl;
    ret = t0.run([](int x) -> int {
        std::cout << x << std::endl;
        return x * 22;
    }, 7);
    std::cout << ret.get() << std::endl;

    return 0;
}