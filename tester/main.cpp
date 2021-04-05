#include "tester/testers.h"

#include "common/assert_exception.h"
#include "common/base.h"

#include <iostream>

int main(int nargs, char **pargs) {
  std::string tester_mode;
  if (nargs >= 2) {
    tester_mode = pargs[1];
  } else {
    std::cout << "Input tester mode to run:" << std::endl;
    std::cin >> tester_mode;
  }

  try {
    if (tester_mode == "binary_search_tree")
      Assert(TestBinarySearchTree(false));
    else if (tester_mode == "convergent")
      Assert(TestContinuedFractionConvergent());
    else if (tester_mode == "find_primes_for_modular_fft")
      FindPrimesForModularFFT(10);
    else if (tester_mode == "generating_function")
      Assert(TestGeneratingFunction());
    else if (tester_mode == "graph_distance_positive_cost")
      Assert(TestGraphEIDistancePositiveCost(false));
    else if (tester_mode == "heap")
      Assert(TestHeap(false));
    else if (tester_mode == "interpolation")
      Assert(TestInterpolation());
    else if (tester_mode == "long_mult")
      Assert(TestLongMult());
    else if (tester_mode == "lowest_common_ancestor")
      Assert(TestLowestCommonAncestor(false));
    else if (tester_mode == "mertens")
      Assert(TestMertens());
    else if (tester_mode == "mertens_compact")
      Assert(TestMertensCompact());
    else if (tester_mode == "minimum_spanning_tree")
      Assert(TestMinimumSpanningTree(false));
    else if (tester_mode == "modular_fft")
      Assert(TestModularFFT());
    else if (tester_mode == "primes_count")
      Assert(TestPrimesCount(false));
    else if (tester_mode == "primes_generation")
      Assert(TestPrimesGeneration(false));
    else if (tester_mode == "range_minimum_query")
      Assert(TestRangeMinimumQuery(false));
    else if (tester_mode == "time_binary_search_tree")
      Assert(TestBinarySearchTree(true));
    else if (tester_mode == "time_disjoint_set")
      Assert(TestDisjointSet());
    else if (tester_mode == "time_graph_distance_positive_cost")
      Assert(TestGraphEIDistancePositiveCost(true));
    else if (tester_mode == "time_heap")
      Assert(TestHeap(true));
    else if (tester_mode == "time_lowest_common_ancestor")
      Assert(TestLowestCommonAncestor(true));
    else if (tester_mode == "time_matrix_mult")
      Assert(TestMatrixMult());
    else if (tester_mode == "time_minimum_spanning_tree")
      Assert(TestMinimumSpanningTree(true));
    else if (tester_mode == "time_primes_count")
      Assert(TestPrimesCount(true));
    else if (tester_mode == "time_primes_generation")
      Assert(TestPrimesGeneration(true));
    else if (tester_mode == "time_range_minimum_query")
      Assert(TestRangeMinimumQuery(true));
    else if (tester_mode == "time_tree_path_maxima")
      Assert(TestTreePathMaxima(true));
    else if (tester_mode == "tree_path_maxima")
      Assert(TestTreePathMaxima(false));
    else
      Assert(false, "Unknown tester mode");
  } catch (AssertException) {
    std::cout << "Test failed." << std::endl;
    return -1;
  }

  std::cout << "Done." << std::endl;
  return 0;
}
