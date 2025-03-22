#include "tester/tester_tree_path_maxima.h"

#include "common/base.h"
#include "common/graph/tree_ei/assign_cost_to_nodes.h"
#include "common/graph/tree_ei/create_hrandom_tree.h"
#include "common/graph/tree_ei/tpm/full_branching_tree.h"
#include "common/graph/tree_ei/tpm/tpm_hld.h"
#include "common/graph/tree_ei/tpm/tpm_pbst.h"
#include "common/graph/tree_ei/tpm/tpm_pbst_fbt.h"
#include "common/timer.h"
#include "common/vector/hrandom_pair.h"
#include "common/vector/sum.h"

#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

TesterTreePathMaxima::TesterTreePathMaxima(unsigned tree_size, unsigned ntrees,
                                           unsigned npaths) {
  trees.resize(ntrees);
  for (unsigned i = 0; i < ntrees; ++i)
    trees[i] = CreateHRandomTree<uint64_t>(tree_size, (1u << 30), i);
  paths = nvector::HRandomPair<unsigned>(npaths, 0, tree_size);
}

size_t TesterTreePathMaxima::TestHLD() const {
  Timer t;
  size_t total_cost = 0;
  std::vector<uint64_t> v;
  for (const auto& t : trees) {
    auto nodes_values = graph::AssignCostToNodes(t, edge_proxy);
    v = graph::tpm::TPM_HLD(t, nodes_values, paths, true);
    total_cost += nvector::Sum(v);
  }
  std::cout << "Test results  [HLD    ]: " << total_cost << "\t"
            << t.get_milliseconds() << std::endl;
  return total_cost;
}

size_t TesterTreePathMaxima::TestHLDFBT() const {
  Timer t;
  TreeGraph ttemp;
  size_t total_cost = 0;
  std::vector<uint64_t> v, nodes_values;
  for (const auto& t : trees) {
    graph::tpm::FullBranchingTree(t, edge_proxy, ttemp, nodes_values);
    v = graph::tpm::TPM_HLD(ttemp, nodes_values, paths, true);
    total_cost += nvector::Sum(v);
  }
  std::cout << "Test results  [HLD FBT]: " << total_cost << "\t"
            << t.get_milliseconds() << std::endl;
  return total_cost;
}

size_t TesterTreePathMaxima::TestPBST() const {
  Timer t;
  size_t total_cost = 0;
  std::vector<uint64_t> v;
  for (const auto& t : trees) {
    auto nodes_values = graph::AssignCostToNodes(t, edge_proxy);
    v = graph::tpm::TPM_PBST(t, nodes_values, paths, true);
    total_cost += nvector::Sum(v);
  }
  std::cout << "Test results  [PT     ]: " << total_cost << "\t"
            << t.get_milliseconds() << std::endl;
  return total_cost;
}

size_t TesterTreePathMaxima::TestPBSTFBT() const {
  Timer t;
  TreeGraph ttemp;
  size_t total_cost = 0;
  std::vector<uint64_t> v, nodes_values;
  for (const auto& t : trees) {
    graph::tpm::FullBranchingTree(t, edge_proxy, ttemp, nodes_values);
    v = graph::tpm::TPM_PBST(ttemp, nodes_values, paths, true);
    total_cost += nvector::Sum(v);
  }
  std::cout << "Test results  [PT  FBT]: " << total_cost << "\t"
            << t.get_milliseconds() << std::endl;
  return total_cost;
}

size_t TesterTreePathMaxima::TestPBST2() const {
  Timer t;
  size_t total_cost = 0;
  std::vector<uint64_t> v;
  for (const auto& t : trees) {
    v = graph::tpm::TPM_PBST_FBT(t, edge_proxy, paths);
    total_cost += nvector::Sum(v);
  }
  std::cout << "Test results  [PT2    ]: " << total_cost << "\t"
            << t.get_milliseconds() << std::endl;
  return total_cost;
}

bool TesterTreePathMaxima::TestAll() {
  std::unordered_set<size_t> hs;
  hs.insert(TestHLD());
  hs.insert(TestHLDFBT());
  hs.insert(TestPBST());
  hs.insert(TestPBSTFBT());
  hs.insert(TestPBST2());
  return hs.size() == 1;
}

bool TestTreePathMaxima(bool time_test) {
  TesterTreePathMaxima t(time_test ? 100000 : 100, 10,
                         time_test ? 1000000 : 1000);
  return t.TestAll();
}
