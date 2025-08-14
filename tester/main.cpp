#include "tester/binary_search_tree/tester.h"
#include "tester/testers.h"

#include "common/assert_exception.h"
#include "common/base.h"

#include <iostream>

int main(int nargs, char **pargs) {
  std::string tester_mode;
  std::string implementation_filter;
  if (nargs >= 2) {
    tester_mode = pargs[1];
    if (nargs >= 3) implementation_filter = pargs[2];
  } else {
    std::cout << "Input tester mode to run:" << std::endl;
    std::cin >> tester_mode;
  }

  try {
    if (tester_mode == "binary_search_tree") {
      assert_exception(TestBinarySearchTree(false));
    } else if (tester_mode == "bst_base") {
      assert_exception(tester::bst::test(tester::bst::TestType::kBase,
                                         implementation_filter));
    } else if (tester_mode == "bst_expensive_data") {
      assert_exception(tester::bst::test(tester::bst::TestType::kExpensiveData,
                                         implementation_filter));
    } else if (tester_mode == "bst_small") {
      assert_exception(tester::bst::test(tester::bst::TestType::kSmall,
                                         implementation_filter));
    } else if (tester_mode == "bst_split_join") {
      assert_exception(tester::bst::test(tester::bst::TestType::kSplitJoin,
                                         implementation_filter));
    } else if (tester_mode == "convergent") {
      assert_exception(TestContinuedFractionConvergent());
    } else if (tester_mode == "find_primes_for_modular_fft") {
      FindPrimesForModularFFT(10);
    } else if (tester_mode == "fixed_universe_successor") {
      assert_exception(TestFixedUniverseSuccessor(false));
    } else if (tester_mode == "generating_function") {
      assert_exception(TestGeneratingFunction());
    } else if (tester_mode == "graph_distance") {
      assert_exception(TestGraphEIDistance(false));
    } else if (tester_mode == "graph_distance_unsigned") {
      assert_exception(TestGraphEIDistanceUnsigned(false));
    } else if (tester_mode == "graph_distance_positive_cost") {
      assert_exception(TestGraphEIDistancePositiveCost(false));
    } else if (tester_mode == "graph_dynamic_connectivity") {
      assert_exception(TestGraphDynamicConnectivity(false));
    } else if (tester_mode == "heap_base") {
      assert_exception(TestHeapBase(false));
    } else if (tester_mode == "heap_ext") {
      assert_exception(TestHeapExt(false));
    } else if (tester_mode == "interpolation") {
      assert_exception(TestInterpolation());
    } else if (tester_mode == "long_mult") {
      assert_exception(TestLongMult());
    } else if (tester_mode == "lowest_common_ancestor") {
      assert_exception(TestLowestCommonAncestor(false));
    } else if (tester_mode == "mertens") {
      assert_exception(TestMertens());
    } else if (tester_mode == "mertens_compact") {
      assert_exception(TestMertensCompact());
    } else if (tester_mode == "minimum_spanning_tree") {
      assert_exception(TestMinimumSpanningTree(false));
    } else if (tester_mode == "modular_fft") {
      assert_exception(TestModularFFT());
    } else if (tester_mode == "primes_count") {
      assert_exception(TestPrimesCount(false));
    } else if (tester_mode == "primes_generation") {
      assert_exception(TestPrimesGeneration(false));
    } else if (tester_mode == "range_minimum_query") {
      assert_exception(TestRangeMinimumQuery(false));
    } else if (tester_mode == "time_binary_search_tree") {
      assert_exception(TestBinarySearchTree(true));
    } else if (tester_mode == "time_disjoint_set") {
      assert_exception(TestDisjointSet());
    } else if (tester_mode == "time_fixed_universe_successor") {
      assert_exception(TestFixedUniverseSuccessor(true));
    } else if (tester_mode == "time_graph_distance") {
      assert_exception(TestGraphEIDistance(true));
    } else if (tester_mode == "time_graph_distance_unsigned") {
      assert_exception(TestGraphEIDistanceUnsigned(true));
    } else if (tester_mode == "time_graph_distance_positive_cost") {
      assert_exception(TestGraphEIDistancePositiveCost(true));
    } else if (tester_mode == "time_graph_dynamic_connectivity") {
      assert_exception(TestGraphDynamicConnectivity(true));
    } else if (tester_mode == "time_heap_base") {
      assert_exception(TestHeapBase(true));
    } else if (tester_mode == "time_heap_ext") {
      assert_exception(TestHeapExt(true));
    } else if (tester_mode == "time_lowest_common_ancestor") {
      assert_exception(TestLowestCommonAncestor(true));
    } else if (tester_mode == "time_matrix_mult") {
      assert_exception(TestMatrixMult());
    } else if (tester_mode == "time_minimum_spanning_tree") {
      assert_exception(TestMinimumSpanningTree(true));
    } else if (tester_mode == "time_primes_count") {
      assert_exception(TestPrimesCount(true));
    } else if (tester_mode == "time_primes_generation") {
      assert_exception(TestPrimesGeneration(true));
    } else if (tester_mode == "time_range_minimum_query") {
      assert_exception(TestRangeMinimumQuery(true));
    } else if (tester_mode == "time_tree_path_maxima") {
      assert_exception(TestTreePathMaxima(true));
    } else if (tester_mode == "tree_path_maxima") {
      assert_exception(TestTreePathMaxima(false));
    } else {
      assert_exception(false, "Unknown tester mode");
    }
  } catch (const AssertException &) {
    std::cout << "Test failed." << std::endl;
    return -1;
  }

  std::cout << "Done." << std::endl;
  return 0;
}
