#include <iostream>
#include "threadpool.hpp"
// #include "reusable_thread.hpp"

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

	// thr::Reusable_thread t([](int x, int y) -> int {
	// 	std::cout << x << std::endl;
	// 	return x * y;
	// });
	// t.run(2, 2);
	// std::cout << t.wait() << std::endl;

	// t.run(2, 2);
	// std::cout << t.wait() << std::endl;

	// t.run(2, 2);
	// std::cout << t.wait() << std::endl;

	thr::Persistent_thread t{ thr::Static_context(f) };
	// auto ret = t.run(2, 2);

	// std::cout << ret.get() << std::endl;

	return 0;
}