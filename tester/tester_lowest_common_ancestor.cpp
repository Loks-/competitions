#include "tester/tester_lowest_common_ancestor.h"

#include "common/base.h"
#include "common/graph/tree/create_hrandom_tree.h"
#include "common/graph/tree/lca/euler_tour.h"
#include "common/graph/tree/lca/offline_proxy.h"
#include "common/graph/tree/lca/schieber_vishkin.h"
#include "common/graph/tree/lca/tarjan_offline.h"
#include "common/hash.h"
#include "common/timer.h"
#include "common/vector/hrandom_pair.h"
#include "common/vector/rmq.h"
#include "common/vector/rmq/ppt_rmq1.h"

#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

TesterLowestCommonAncestor::TesterLowestCommonAncestor(unsigned tree_size,
                                                       unsigned ntrees,
                                                       unsigned nqueries) {
  trees.resize(ntrees);
  for (unsigned i = 0; i < ntrees; ++i)
    trees[i] = CreateHRandomTree(tree_size, i);
  queries = nvector::HRandomPair<unsigned>(nqueries, 0, tree_size);
}

size_t TesterLowestCommonAncestor::TestTarjanOffline() const {
  Timer t;
  size_t h = 0;
  std::vector<uint64_t> r;
  for (const auto& t : trees) {
    auto vr = graph::lca::TarjanOffline(t, queries);
    for (auto r : vr) h = HashCombine(h, r);
  }
  std::cout << "Test results  [Tarjan Offline  ]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

size_t TesterLowestCommonAncestor::TestSchieberVishkin() const {
  Timer t;
  size_t h = 0;
  std::vector<uint64_t> r;
  for (const auto& t : trees) {
    auto vr = graph::lca::OfflineProxy<graph::lca::SchieberVishkin>(t, queries);
    for (auto r : vr) h = HashCombine(h, r);
  }
  std::cout << "Test results  [Schieber Vishkin]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

size_t TesterLowestCommonAncestor::TestEulerTourRMQ() const {
  Timer t;
  size_t h = 0;
  std::vector<uint64_t> r;
  for (const auto& t : trees) {
    auto vr =
        graph::lca::OfflineProxy<graph::lca::EulerTour<nvector::RMQ<unsigned>>>(
            t, queries);
    for (auto r : vr) h = HashCombine(h, r);
  }
  std::cout << "Test results  [Euler Tour RMQ  ]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

size_t TesterLowestCommonAncestor::TestEulerTourRMQ1() const {
  Timer t;
  size_t h = 0;
  std::vector<uint64_t> r;
  for (const auto& t : trees) {
    auto vr = graph::lca::OfflineProxy<
        graph::lca::EulerTour<nvector::rmq::PPTRMQ1<unsigned>>>(t, queries);
    for (auto r : vr) h = HashCombine(h, r);
  }
  std::cout << "Test results  [Euler Tour RMQ1 ]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterLowestCommonAncestor::TestAll() {
  std::unordered_set<size_t> hs;
  hs.insert(TestTarjanOffline());
  hs.insert(TestSchieberVishkin());
  hs.insert(TestEulerTourRMQ());
  hs.insert(TestEulerTourRMQ1());
  return hs.size() == 1;
}

bool TestLowestCommonAncestor(bool time_test) {
  TesterLowestCommonAncestor t(time_test ? 1000000 : 100, 10,
                               time_test ? 1000000 : 1000);
  return t.TestAll();
}
