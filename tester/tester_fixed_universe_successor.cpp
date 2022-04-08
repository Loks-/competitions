#include "tester/tester_fixed_universe_successor.h"

#include "common/data_structures/fixed_universe_successor/binary_search_tree.h"
#include "common/data_structures/fixed_universe_successor/btrie.h"
#include "common/data_structures/fixed_universe_successor/fixed_length_set_6b.h"
#include "common/data_structures/fixed_universe_successor/fixed_length_set_8b.h"
#include "common/data_structures/fixed_universe_successor/implicit_binary_tree.h"
#include "common/data_structures/fixed_universe_successor/multi_search_tree.h"
#include "common/data_structures/fixed_universe_successor/sqrt_decomposition.h"
#include "common/data_structures/fixed_universe_successor/two_layers.h"
#include "common/data_structures/fixed_universe_successor/van_emde_boas_tree_compact.h"
#include "common/data_structures/fixed_universe_successor/van_emde_boas_tree_compact2.h"
#include "common/data_structures/fixed_universe_successor/van_emde_boas_tree_compact_static_size.h"
#include "common/data_structures/fixed_universe_successor/van_emde_boas_tree_full.h"
#include "common/data_structures/fixed_universe_successor/van_emde_boas_tree_full_static_size.h"
#include "common/data_structures/fixed_universe_successor/vector_multiset.h"
#include "common/data_structures/fixed_universe_successor/vector_precomputed.h"
#include "common/data_structures/fixed_universe_successor/vector_set.h"
#include "common/data_structures/fixed_universe_successor/x_fast_trie.h"
#include "common/hash.h"
#include "common/timer.h"
#include "common/vector/hrandom.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

TesterFixedUniverseSuccessor::TesterFixedUniverseSuccessor(size_t _usize,
                                                           size_t ssize)
    : usize(_usize) {
  vdata = nvector::HRandom(ssize, 0, usize);
  vdata.insert(vdata.end(), vdata.begin(), vdata.end());
  std::random_shuffle(vdata.begin(), vdata.end());
}

template <class TFUS>
size_t TesterFixedUniverseSuccessor::TestBase(const std::string& name) const {
  Timer t;
  size_t h = 0;
  TFUS fus;
  fus.Init(usize);
  for (size_t x : vdata) {
    // std::cout << x << " " << fus.Successor(x) << " " << fus.Predecessor(x)
    //           << std::endl;
    h = HashCombine(h, fus.Min());
    h = HashCombine(h, fus.Max());
    h = HashCombine(h, fus.Successor(x));
    h = HashCombine(h, fus.Predecessor(x));
    if (fus.HasKey(x)) {
      fus.Delete(x);
    } else {
      fus.Insert(x);
    }
  }
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterFixedUniverseSuccessor::TestAll(bool small_test) const {
  std::cout << "USize = " << usize << std::endl;
  std::unordered_set<size_t> hs;
  if (small_test) {
    hs.insert(TestBase<ds::fus::VectorSet>("VSet   "));
    hs.insert(TestBase<ds::fus::VectorMultiset>("VMSet  "));
    hs.insert(TestBase<ds::fus::VectorPrecomputed>("VPreC  "));
  }
  if (usize <= (1ull << 30)) {
    hs.insert(TestBase<ds::fus::TwoLayers<ds::fus::FLSetB6>>("L2   B6"));
    hs.insert(TestBase<ds::fus::TwoLayers<ds::fus::FLSetB8>>("L2   B8"));
    hs.insert(TestBase<ds::fus::SqrtDecomposition>("SqrtD  "));
    hs.insert(TestBase<ds::fus::ImplicitBinaryTree>("IBT    "));
  }
  hs.insert(TestBase<ds::fus::BinarySearchTree>("BST    "));
  hs.insert(TestBase<ds::fus::BTrie<ds::fus::FLSetB6>>("BTrie 6"));
  hs.insert(TestBase<ds::fus::BTrie<ds::fus::FLSetB8>>("BTrie 8"));
  hs.insert(TestBase<ds::fus::MultiSearchTree<ds::fus::FLSetB6>>("MST  B6"));
  hs.insert(TestBase<ds::fus::MultiSearchTree<ds::fus::FLSetB8>>("MST  B8"));
  if (usize <= (1ull << 27)) {
    hs.insert(TestBase<ds::fus::VanEmdeBoasTreeFull>("VEBTF  "));
  }
  if (usize <= (1ull << 20)) {
    hs.insert(TestBase<ds::fus::VanEmdeBoasTreeFullStaticSize<20>>("VEBTFS "));
  } else if (usize <= (1ull << 27)) {
    hs.insert(TestBase<ds::fus::VanEmdeBoasTreeFullStaticSize<27>>("VEBTFS "));
  }
  hs.insert(TestBase<ds::fus::VanEmdeBoasTreeCompact>("VEBTC  "));
  if (usize <= (1ull << 20)) {
    hs.insert(
        TestBase<ds::fus::VanEmdeBoasTreeCompactStaticSize<20>>("VEBTCS "));
  } else if (usize <= (1ull << 27)) {
    hs.insert(
        TestBase<ds::fus::VanEmdeBoasTreeCompactStaticSize<27>>("VEBTCS "));
  } else {
    hs.insert(
        TestBase<ds::fus::VanEmdeBoasTreeCompactStaticSize<64>>("VEBTCS "));
  }
  if (small_test) {
    hs.insert(TestBase<ds::fus::XFastTrie>("XFTrie "));
  }
  return hs.size() == 1;
}

bool TestFixedUniverseSuccessor(bool time_test) {
  if (time_test) {
    TesterFixedUniverseSuccessor tfus1(100000000, 1000000);
    TesterFixedUniverseSuccessor tfus2((1ull << 60), 1000000);
    // return tfus2.TestAll(false);
    return tfus1.TestAll(false) && tfus2.TestAll(false);
  } else {
    TesterFixedUniverseSuccessor tfus1(100, 1000);
    TesterFixedUniverseSuccessor tfus2(10000, 1000);
    return tfus1.TestAll(true) && tfus2.TestAll(true);
  }
}
