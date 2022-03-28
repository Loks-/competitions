#include "tester/tester_fixed_universe_successor.h"

#include "common/data_structures/fixed_universe_successor/binary_search_tree.h"
#include "common/data_structures/fixed_universe_successor/implicit_binary_tree.h"
#include "common/data_structures/fixed_universe_successor/sqrt_decomposition.h"
#include "common/data_structures/fixed_universe_successor/two_layers_u64.h"
#include "common/data_structures/fixed_universe_successor/vector_multiset.h"
#include "common/data_structures/fixed_universe_successor/vector_precomputed.h"
#include "common/data_structures/fixed_universe_successor/vector_set.h"
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

bool TesterFixedUniverseSuccessor::TestAll() const {
  std::unordered_set<size_t> hs;
  hs.insert(TestBase<ds::fus::VectorSet>("VSet  "));
  hs.insert(TestBase<ds::fus::VectorMultiset>("VMSet "));
  hs.insert(TestBase<ds::fus::VectorPrecomputed>("VPreC "));
  hs.insert(TestBase<ds::fus::TwoLayersU64>("L2U64 "));
  hs.insert(TestBase<ds::fus::SqrtDecomposition>("SqrtD "));
  hs.insert(TestBase<ds::fus::BinarySearchTree>("BST   "));
  hs.insert(TestBase<ds::fus::ImplicitBinaryTree>("IBT   "));
  return hs.size() == 1;
}

bool TestFixedUniverseSuccessor(bool time_test) {
  TesterFixedUniverseSuccessor tfus(time_test ? 100000000 : 10000,
                                    time_test ? 100000 : 1000);
  return tfus.TestAll();
}
