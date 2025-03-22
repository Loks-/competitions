#pragma once

#include "tester/disjoint_set_proxy.h"

#include "common/data_structures/disjoint_set_extended.h"
#include "common/hash/combine.h"
#include "common/timer.h"

#include <iostream>
#include <utility>
#include <vector>

class TesterDisjointSet {
 protected:
  unsigned size, unions, unions_per_block;
  std::vector<std::pair<unsigned, unsigned>> vunions;

 public:
  TesterDisjointSet(unsigned _size, unsigned _unions,
                    unsigned _unions_per_block);
  TesterDisjointSet(unsigned _size, unsigned _unions)
      : TesterDisjointSet(_size, _unions, _size) {}

  template <EPathCompression pc>
  size_t Test() const {
    Timer t;
    size_t h = 0;
    DisjointSetProxy<pc> dsp(size);
    for (unsigned i = 0; i < unions; ++i) {
      if ((i > 0) && ((i % unions_per_block) == 0)) dsp.Init(size);
      dsp.Union(vunions[i].first, vunions[i].second);
      nhash::DCombineH(h, dsp.GetUnions());
    }
    std::cout << "Test results: " << int(pc) << "\t" << h << "\t"
              << t.get_milliseconds() << std::endl;
    return h;
  }

  size_t TestExtended() const;

  void TestAll() const;
};
