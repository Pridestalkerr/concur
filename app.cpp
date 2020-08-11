#include <iostream>
// #include "threadpool.hpp"
#include "reusable_thread.hpp"

int main()
{
	// thr::Thread t;
	// std::function <int(int, int)> f = [](int x, int y) -> int {
	// 	std::cout << x << std::endl;
	// 	return x * y;
	// };

	// std::cout << t.run(f, 2, 2).get();
	// std::cout << t.run(f, 2, 2).get();
	// std::cout << t.run(f, 2, 2).get();
	// std::cout << t.run(f, 2, 2).get();

	std::function <int(int, int)> f = [](int x, int y) -> int {
		std::cout << x << std::endl;
		return x * y;
	};

	thr::Reusable_thread t(f);
	t.run(2, 2);
	std::cout << t.wait() << std::endl;

	t.run(2, 2);
	std::cout << t.wait() << std::endl;

	t.run(2, 2);
	std::cout << t.wait() << std::endl;

	return 0;
}