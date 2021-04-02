#include "tester/tester_tree_path_maxima.h"

#include "common/base.h"
#include "common/graph/tree_ei/create_random_tree.h"
#include "common/graph/tree_ei/assign_cost_to_nodes.h"
#include "common/graph/tree_ei/tpm/full_branching_tree.h"
#include "common/graph/tree_ei/tpm/tpm_hld.h"
#include "common/graph/tree_ei/tpm/tpm_pbst.h"
#include "common/hash.h"
#include "common/timer.h"

#include <iostream>
#include <numeric>
#include <unordered_set>
#include <utility>
#include <vector>

TesterTreePathMaxima::TesterTreePathMaxima(unsigned tree_size, unsigned ntrees,
                                           unsigned npaths) {
  trees.resize(ntrees);
  for (unsigned i = 0; i < ntrees; ++i)
    trees[i] = CreateRandomTree<uint64_t>(tree_size, (1u << 30), i);
  size_t h1 = 0, h2 = 0;
  for (unsigned i = 0; i < npaths; ++i) {
    h1 = HashCombine(h2, i);
    h2 = HashCombine(h1, i);
    paths.push_back(std::make_pair(h1 % tree_size, h2 % tree_size));
  }
}

size_t TesterTreePathMaxima::TestHLD() const {
  Timer t;
  size_t total_cost = 0;
  std::vector<uint64_t> v;
  for (const auto& t : trees) {
    auto nodes_values = graph::AssignCostToNodes(t, edge_proxy);
    v = graph::tpm::TPM_HLD(t, nodes_values, paths, true);
    total_cost += std::accumulate(v.begin(), v.end(), size_t(0));
  }
  std::cout << "Test results  [HLD    ]: " << total_cost << "\t" << t.GetMilliseconds()
            << std::endl;
  return total_cost;
}

size_t TesterTreePathMaxima::TestHLDFBT() const {
  Timer t;
  TreeGraph ttemp;
  size_t total_cost = 0;
  std::vector<uint64_t> v, nodes_values;
  for (const auto& t : trees) {
    graph::tpm::FullBranchingTree(t,edge_proxy, ttemp, nodes_values);
    v = graph::tpm::TPM_HLD(ttemp, nodes_values, paths, true);
    total_cost += std::accumulate(v.begin(), v.end(), size_t(0));
  }
  std::cout << "Test results  [HLD FBT]: " << total_cost << "\t" << t.GetMilliseconds()
            << std::endl;
  return total_cost;
}

size_t TesterTreePathMaxima::TestPBST() const {
  Timer t;
  size_t total_cost = 0;
  std::vector<uint64_t> v;
  for (const auto& t : trees) {
    auto nodes_values = graph::AssignCostToNodes(t, edge_proxy);
    v = graph::tpm::TPM_PBST(t, nodes_values, paths, true);
    total_cost += std::accumulate(v.begin(), v.end(), size_t(0));
  }
  std::cout << "Test results  [PT     ]: " << total_cost << "\t" << t.GetMilliseconds()
            << std::endl;
  return total_cost;
}

bool TesterTreePathMaxima::TestAll() {
  std::unordered_set<size_t> hs;
  hs.insert(TestHLD());
  hs.insert(TestHLDFBT());
  hs.insert(TestPBST());
  return hs.size() == 1;
}

bool TestTreePathMaxima(bool time_test) {
  TesterTreePathMaxima t(time_test ? 1000 : 100, 10, time_test ? 10000 : 1000);
  return t.TestAll();
}
