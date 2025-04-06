#include "tester/binary_search_tree/tester.h"

#include "tester/binary_search_tree/data.h"
#include "tester/binary_search_tree/implementation.h"
#include "tester/binary_search_tree/run_each.h"
#include "tester/binary_search_tree/scenario/build_search.h"

namespace tester {
namespace bst {

bool test(TestType test_type) {
  switch (test_type) {
    case TestType::kSmall: {
      using Scenarios =
          std::tuple<scenario::BuildSearch<DataType::kIncreasing>,
                     scenario::BuildSearch<DataType::kReverse>,
                     scenario::BuildSearch<DataType::kShuffled>,
                     scenario::BuildSearch<DataType::kShuffledDuplicates>>;

      return run_each<
          Scenarios, impl::HKT_HPF_AVL, impl::HKT_HPT_AVL,
          impl::HKT_HPF_Scapegoat, impl::HKT_HPT_Scapegoat, impl::HKF_HPT_Splay,
          impl::HKT_HPT_Splay, impl::HKT_HPF_Static, impl::HKT_HPT_Static,
          impl::HKF_HPF_Treap, impl::HKF_HPT_Treap, impl::HKT_HPF_Treap,
          impl::HKT_HPT_Treap, impl::HKT_HPF_Unbalanced,
          impl::HKT_HPT_Unbalanced, impl::HKT_HPF_WAVL, impl::HKT_HPT_WAVL>(
          1000);
    }

    case TestType::kBase: {
      using Scenarios =
          std::tuple<scenario::BuildSearch<DataType::kIncreasing>,
                     scenario::BuildSearch<DataType::kReverse>,
                     scenario::BuildSearch<DataType::kShuffled>,
                     scenario::BuildSearch<DataType::kShuffledDuplicates>>;

      return run_each<
          Scenarios, impl::HKT_HPF_AVL, impl::HKT_HPT_AVL,
          impl::HKT_HPF_Scapegoat, impl::HKT_HPT_Scapegoat, impl::HKF_HPT_Splay,
          impl::HKT_HPT_Splay, impl::HKT_HPF_Static, impl::HKT_HPT_Static,
          impl::HKF_HPF_Treap, impl::HKF_HPT_Treap, impl::HKT_HPF_Treap,
          impl::HKT_HPT_Treap, impl::HKT_HPF_Unbalanced,
          impl::HKT_HPT_Unbalanced, impl::HKT_HPF_WAVL, impl::HKT_HPT_WAVL>(
          100000);
    }

    default:
      return false;
  }
}

}  // namespace bst
}  // namespace tester
