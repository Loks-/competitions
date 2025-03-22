#include "tester/tester_disjoint_set.h"

#include "tester/disjoint_set_proxy.h"

#include <random>

TesterDisjointSet::TesterDisjointSet(unsigned _size, unsigned _unions,
                                     unsigned _unions_per_block) {
  size = _size;
  unions = _unions;
  unions_per_block = _unions_per_block;
  vunions.reserve(unions);
  std::minstd_rand random_engine;
  std::uniform_int_distribution<unsigned> distribution(0, size - 1);
  for (unsigned i = 0; i < unions; ++i)
    vunions.push_back(
        {distribution(random_engine), distribution(random_engine)});
}

size_t TesterDisjointSet::TestExtended() const {
  Timer t;
  size_t h = 0;
  ds::DisjointSetExtended<unsigned> dsp;
  dsp.Reserve(size);
  for (unsigned i = 0; i < unions; ++i) {
    if ((i > 0) && ((i % unions_per_block) == 0)) dsp.Reset();
    dsp.Union(vunions[i].first, vunions[i].second);
    nhash::DCombineH(h, dsp.GetUnions());
  }
  std::cout << "Test results: E\t" << h << "\t" << t.get_milliseconds()
            << std::endl;
  return h;
}

void TesterDisjointSet::TestAll() const {
  // Test<pc_none>();
  Test<pc_default>();
  Test<pc_compression_recursive>();
  Test<pc_compression_two_runs>();
  Test<pc_compression_stack>();
  Test<pc_halving>();
  Test<pc_splitting>();
  TestExtended();
}

bool TestDisjointSet() {
  TesterDisjointSet tds(100000, 100000 * 5000);
  tds.TestAll();
  return true;
}
