#include "common/base.h"

#include "assert.h"
#include "testers.h"

#include <iostream>

int main(int nargs, char **pargs)
{
	std::string tester_mode;
	if (nargs >= 2)
	{
		tester_mode = pargs[1];
	}
	else
	{
		std::cout << "Input tester mode to run:" << std::endl;
		std::cin >> tester_mode;
	}

	try
	{
		if (tester_mode == "binary_search_tree")
			Assert(TestBinarySearchTree(false));
		else if (tester_mode == "convergent")
			Assert(TestContinuedFractionConvergent());
		else if (tester_mode == "interpolation")
			Assert(TestInterpolation());
		else if (tester_mode == "mertens")
			Assert(TestMertens());
		else if (tester_mode == "modular_fft")
			Assert(TestModularFFT());
		else if (tester_mode == "time_binary_search_tree")
			Assert(TestBinarySearchTree(true));
		else if (tester_mode == "time_disjoint_set")
			Assert(TestDisjointSet());
		else if (tester_mode == "time_matrix_mult")
			Assert(TestMatrixMult());
		else
			Assert(false, "Unknown tester mode");
	}
	catch (AssertException)
	{
		std::cout << "Test failed." << std::endl;
		return -1;
	}

	return 0;
}
