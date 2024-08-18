#include "tester/tester_dynamic_connectivity.h"

#include "common/graph/dynamic/connectivity/base.h"
// #include "common/graph/dynamic/connectivity/spanning_tree_ett_st.h"
#include "common/graph/dynamic/connectivity/holm_lcte.h"
#include "common/graph/dynamic/connectivity/spanning_tree_lct.h"
#include "common/hash/combine.h"
#include "common/timer.h"
#include "common/vector/hrandom.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

TesterDynamicConnectivity::TesterDynamicConnectivity(unsigned _gsize,
                                                     unsigned _max_edges,
                                                     unsigned _main_loop) {
  gsize = _gsize;
  max_edges = _max_edges;
  main_loop = _main_loop;
  vsf = nvector::HRandom<unsigned>(max_edges, 1, gsize);
  vst = nvector::HRandom<unsigned>(max_edges, 2, gsize - 1);
  for (unsigned i = 0; i < max_edges; ++i) {
    if (vst[i] >= vsf[i]) ++vst[i];
  }
  vme = nvector::HRandom<unsigned>(main_loop, 3, max_edges);
  vmf = nvector::HRandom<unsigned>(main_loop, 4, gsize);
  vmt = nvector::HRandom<unsigned>(main_loop, 5, gsize - 1);
  for (unsigned i = 0; i < main_loop; ++i) {
    if (vmt[i] >= vmf[i]) ++vmt[i];
  }
}

template <class TSolver>
size_t TesterDynamicConnectivity::Test(const std::string& name) const {
  Timer t;
  TSolver s(gsize);
  size_t h = 0;
  std::vector<typename TSolver::TEdgeID> edges;
  for (unsigned i = 0; i < max_edges; ++i) {
    auto e = s.InsertEdge(vsf[i], vst[i]);
    edges.push_back(e);
    // std::cout << "\t" << s.Components() << std::endl;
    nhash::DCombineH(h, s.Components());
  }
  for (unsigned i = 0; i < main_loop; ++i) {
    auto eindex = vme[i];
    s.RemoveEdge(edges[eindex]);
    edges[eindex] = s.InsertEdge(vmf[i], vmt[i]);
    // std::cout << "\t" << s.Components() << std::endl;
    nhash::DCombineH(h, s.Components());
  }
  for (unsigned i = 0; i < max_edges; ++i) {
    s.RemoveEdge(edges[i]);
    // std::cout << "\t" << s.Components() << std::endl;
    nhash::DCombineH(h, s.Components());
  }
  std::cout << "Test results  [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterDynamicConnectivity::TestAll() {
  std::unordered_set<uint64_t> hs;
  hs.insert(Test<graph::dynamic::connectivity::Base>("Base     "));
  // hs.insert(Test<graph::dynamic::connectivity::SpanningTreeETTST>("ST ETT
  // ST"));
  hs.insert(Test<graph::dynamic::connectivity::SpanningTreeLCT>("ST LCT   "));
  hs.insert(Test<graph::dynamic::connectivity::HolmLCTE>("Holm LCTE"));
  return hs.size() == 1;
}

bool TestGraphDynamicConnectivity(bool time_test) {
  TesterDynamicConnectivity t(time_test ? 10000 : 1000,
                              time_test ? 10000 : 1000,
                              time_test ? 10000 : 2000);
  return t.TestAll();
}
