#include <iostream>
#include "./include/async.hpp"

int main()
{

    std::function <int(int, int)> f = [](int x, int y) -> int {
        std::cout << x << std::endl;
        return x * y;
    };

    thr::Async async;    // make sure this doesnt go out of scope!
    
    std::cout <<

    async(f, 2, 2).get()

    << std::endl;

    return 0;
}