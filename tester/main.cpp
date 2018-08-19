#include "common/base.h"

#include "testers.h"

#include <iostream>

int main()
{
	std::cout << "Hello from tester." << std::endl;
	assert(TestBinarySearchTree(false));
	// assert(TestDisjointSet());
	return 0;
}
