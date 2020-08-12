#include <iostream>
#include "async.hpp"

int main()
{
	thr::Async async;    // make sure this doesnt go out of scope!
	
	std::cout <<

	async([](int x, int y) -> int {
		std::cout << x << std::endl;
		return x * y;
	}, 2, 2).get()

	<< std::endl;

	return 0;
}