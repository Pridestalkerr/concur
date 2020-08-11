#include <iostream>
#include "threadpool.hpp"
#include "Promise.hpp"

int main()
{
	thr::Thread t;
	std::function <int(int, int)> f = [](int x, int y) -> int {
		std::cout << x << std::endl;
		return x * y;
	};

	std::cout << t.run(f, 2, 2).get();
	std::cout << t.run(f, 2, 2).get();
	std::cout << t.run(f, 2, 2).get();
	std::cout << t.run(f, 2, 2).get();

	return 0;
}