#include "tester/tester_range_minimum_query.h"

#include "common/base.h"
#include "common/hash.h"
#include "common/timer.h"
#include "common/vector/hrandom.h"
#include "common/vector/hrandom_pair.h"
#include "common/vector/rmq/ppt.h"
#include "common/vector/rmq/segment_tree.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

RangeMinimumQuery::RangeMinimumQuery(unsigned size, unsigned ntests,
                                     unsigned nqueries) {
  vv.resize(ntests);
  for (unsigned i = 0; i < ntests; ++i) vv[i] = nvector::HRandom(size, i);
  queries = nvector::HRandomPair<unsigned>(nqueries, 0, size);
}

template <class TSolver>
size_t RangeMinimumQuery::Test(const std::string& solver_name) const {
  Timer t;
  size_t h = 0;
  for (const auto& v : vv) {
    TSolver s(v);
    for (auto q : queries) {
      h += s.Minimum(std::min(q.first, q.second),
                     std::max(q.first, q.second) + 1)
               .value;
    }
  }
  std::cout << "Test results  [" << solver_name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool RangeMinimumQuery::TestAll() {
  std::unordered_set<size_t> hs;
  hs.insert(Test<nvector::rmq::SegmentTree<size_t>>("ST "));
  hs.insert(Test<nvector::rmq::PPT<size_t>>("PPT"));
  return hs.size() == 1;
}

bool TestRangeMinimumQuery(bool time_test) {
  RangeMinimumQuery t(time_test ? 1000000 : 100, 10,
                      time_test ? 1000000 : 1000);
  return t.TestAll();
}
