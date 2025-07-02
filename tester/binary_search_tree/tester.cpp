#include "tester/binary_search_tree/tester.h"

#include "tester/binary_search_tree/data.h"
#include "tester/binary_search_tree/implementation.h"
#include "tester/binary_search_tree/run_each.h"
#include "tester/binary_search_tree/scenario/build.h"
#include "tester/binary_search_tree/scenario/build_add_as.h"
#include "tester/binary_search_tree/scenario/build_add_each.h"
#include "tester/binary_search_tree/scenario/insert_at_remove_at.h"
#include "tester/binary_search_tree/scenario/insert_at_remove_at_add_as.h"
#include "tester/binary_search_tree/scenario/insert_at_remove_at_add_each.h"
#include "tester/binary_search_tree/scenario/insert_remove.h"
#include "tester/binary_search_tree/scenario/insert_remove_add_as.h"
#include "tester/binary_search_tree/scenario/insert_remove_add_each.h"
#include "tester/binary_search_tree/scenario/insert_remove_node.h"
#include "tester/binary_search_tree/scenario/insert_remove_node_add_as.h"
#include "tester/binary_search_tree/scenario/insert_remove_node_add_each.h"

namespace tester {
namespace bst {

bool test(TestType test_type) {
  switch (test_type) {
    case TestType::kSmall: {
      using Scenarios = std::tuple<
          scenario::Build<DataType::kIncreasing>,
          scenario::Build<DataType::kReverse>,
          scenario::Build<DataType::kShuffled>,
          scenario::Build<DataType::kShuffledDuplicates>,
          scenario::InsertRemove<DataType::kIncreasing>,
          scenario::InsertRemove<DataType::kReverse>,
          scenario::InsertRemove<DataType::kShuffled>,
          scenario::InsertRemove<DataType::kShuffledDuplicates>,
          scenario::InsertRemoveNode<DataType::kIncreasing>,
          scenario::InsertRemoveNode<DataType::kReverse>,
          scenario::InsertRemoveNode<DataType::kShuffled>,
          scenario::InsertRemoveNode<DataType::kShuffledDuplicates>,
          scenario::InsertAtRemoveAt<DataType::kRandom>,
          scenario::BuildAddEach<DataType::kShuffled>,
          scenario::BuildAddEach<DataType::kShuffledDuplicates>,
          scenario::InsertRemoveAddEach<DataType::kIncreasing>,
          scenario::InsertRemoveAddEach<DataType::kReverse>,
          scenario::InsertRemoveAddEach<DataType::kShuffled>,
          scenario::InsertRemoveNodeAddEach<DataType::kIncreasing>,
          scenario::InsertRemoveNodeAddEach<DataType::kReverse>,
          scenario::InsertRemoveNodeAddEach<DataType::kShuffled>,
          scenario::InsertRemoveNodeAddEach<DataType::kShuffledDuplicates>,
          scenario::InsertAtRemoveAtAddEach<DataType::kRandom>,
          scenario::BuildAddAS<DataType::kShuffled>,
          scenario::BuildAddAS<DataType::kShuffledDuplicates>,
          scenario::InsertRemoveAddAS<DataType::kIncreasing>,
          scenario::InsertRemoveAddAS<DataType::kReverse>,
          scenario::InsertRemoveAddAS<DataType::kShuffled>,
          scenario::InsertRemoveNodeAddAS<DataType::kIncreasing>,
          scenario::InsertRemoveNodeAddAS<DataType::kReverse>,
          scenario::InsertRemoveNodeAddAS<DataType::kShuffled>,
          scenario::InsertRemoveNodeAddAS<DataType::kShuffledDuplicates>,
          scenario::InsertAtRemoveAtAddAS<DataType::kRandom>>;

      return run_each<
          true, Scenarios, impl::HKF_HPF_AVL, impl::HKF_HPT_AVL,
          impl::HKT_HPF_AVL, impl::HKT_HPT_AVL, impl::HKF_HPT_RedBlack,
          impl::HKT_HPT_RedBlack, impl::HKF_HPF_Scapegoat,
          impl::HKF_HPT_Scapegoat, impl::HKT_HPF_Scapegoat,
          impl::HKT_HPT_Scapegoat, impl::HKF_HPT_Splay, impl::HKT_HPT_Splay,
          impl::HKF_HPF_Static, impl::HKF_HPT_Static, impl::HKT_HPF_Static,
          impl::HKT_HPT_Static, impl::HKF_HPF_Treap, impl::HKF_HPT_Treap,
          impl::HKT_HPF_Treap, impl::HKT_HPT_Treap, impl::HKF_HPF_Unbalanced,
          impl::HKF_HPT_Unbalanced, impl::HKT_HPF_Unbalanced,
          impl::HKT_HPT_Unbalanced, impl::HKF_HPF_WAVL, impl::HKF_HPT_WAVL,
          impl::HKT_HPT_WAVL>(1000);
    }

    case TestType::kBase: {
      using Scenarios =
          std::tuple<scenario::Build<DataType::kIncreasing>,
                     scenario::Build<DataType::kReverse>,
                     scenario::Build<DataType::kShuffled>,
                     scenario::Build<DataType::kShuffledDuplicates>,
                     scenario::InsertRemove<DataType::kIncreasing>,
                     scenario::InsertRemove<DataType::kReverse>,
                     scenario::InsertRemove<DataType::kShuffled>,
                     scenario::InsertRemove<DataType::kShuffledDuplicates>,
                     scenario::BuildAddEach<DataType::kShuffled>,
                     scenario::BuildAddEach<DataType::kShuffledDuplicates>,
                     scenario::InsertRemoveAddEach<DataType::kIncreasing>,
                     scenario::InsertRemoveAddEach<DataType::kReverse>,
                     scenario::InsertRemoveAddEach<DataType::kShuffled>>;

      return run_each<
          false, Scenarios, impl::HKF_HPF_AVL, impl::HKF_HPT_AVL,
          impl::HKT_HPF_AVL, impl::HKT_HPT_AVL, impl::HKF_HPT_RedBlack,
          impl::HKT_HPT_RedBlack, impl::HKF_HPF_Scapegoat,
          impl::HKF_HPT_Scapegoat, impl::HKT_HPF_Scapegoat,
          impl::HKT_HPT_Scapegoat, impl::HKF_HPT_Splay, impl::HKT_HPT_Splay,
          impl::HKF_HPF_Treap, impl::HKF_HPT_Treap, impl::HKT_HPF_Treap,
          impl::HKT_HPT_Treap, impl::HKF_HPF_WAVL, impl::HKF_HPT_WAVL,
          impl::HKT_HPT_WAVL>(100000);
    }

    case TestType::kExpensiveData: {
      using Scenarios =
          std::tuple<scenario::BuildAddAS<DataType::kShuffled>,
                     scenario::BuildAddAS<DataType::kShuffledDuplicates>,
                     scenario::InsertRemoveAddAS<DataType::kIncreasing>,
                     scenario::InsertRemoveAddAS<DataType::kReverse>,
                     scenario::InsertRemoveAddAS<DataType::kShuffled>>;

      return run_each<
          false, Scenarios, impl::HKF_HPF_AVL, impl::HKF_HPT_AVL,
          impl::HKT_HPF_AVL, impl::HKT_HPT_AVL, impl::HKF_HPT_RedBlack,
          impl::HKT_HPT_RedBlack, impl::HKF_HPF_Scapegoat,
          impl::HKF_HPT_Scapegoat, impl::HKT_HPF_Scapegoat,
          impl::HKT_HPT_Scapegoat, impl::HKF_HPT_Splay, impl::HKT_HPT_Splay,
          impl::HKF_HPF_Treap, impl::HKF_HPT_Treap, impl::HKT_HPF_Treap,
          impl::HKT_HPT_Treap, impl::HKF_HPF_WAVL, impl::HKF_HPT_WAVL,
          impl::HKT_HPT_WAVL>(100000);
    }

    default:
      return false;
  }
}

}  // namespace bst
}  // namespace tester
