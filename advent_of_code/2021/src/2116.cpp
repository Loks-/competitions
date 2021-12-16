#include "common/numeric/utils/hex.h"
#include "common/stl/base.h"
#include "common/vector/max.h"
#include "common/vector/min.h"
#include "common/vector/product.h"
#include "common/vector/read_lines.h"
#include "common/vector/sum.h"

#include <functional>

int main_2116() {
  auto s0 = nvector::ReadLines()[0];
  vector<unsigned> vd;
  for (auto c : s0) {
    auto k = HexToU(c);
    for (unsigned i = 0; i < 4; ++i) vd.push_back((k >> (3 - i)) & 1);
  }
  unsigned index = 0;

  auto ReadU = [&](unsigned l) {
    unsigned x = 0;
    for (; l; --l) x = 2 * x + vd[index++];
    return x;
  };

  auto ReadL = [&]() {
    int64_t x = 0;
    for (; vd[index++];) x = 16 * x + ReadU(4);
    x = 16 * x + ReadU(4);
    return x;
  };

  unsigned versions_sum = 0;
  std::function<uint64_t()> Solve = [&]() -> uint64_t {
    unsigned version = ReadU(3);
    unsigned type = ReadU(3);
    versions_sum += version;
    if (type == 4) return ReadL();
    vector<int64_t> vm;
    if (vd[index++]) {
      auto n = ReadU(11);
      for (unsigned i = 0; i < n; ++i) vm.push_back(Solve());
    } else {
      auto n = ReadU(15);
      auto index_end = index + n;
      for (; index < index_end;) vm.push_back(Solve());
      if (index != index_end) cout << "Ops" << endl;
    }
    if (type == 0) {
      return nvector::Sum(vm);
    } else if (type == 1) {
      return nvector::Product(vm);
    } else if (type == 2) {
      return nvector::Min(vm);
    } else if (type == 3) {
      return nvector::Max(vm);
    } else if (type == 5) {
      return (vm[0] > vm[1]) ? 1 : 0;
    } else if (type == 6) {
      return (vm[0] < vm[1]) ? 1 : 0;
    } else if (type == 7) {
      return (vm[0] == vm[1]) ? 1 : 0;
    } else {
      cout << "Ops" << endl;
      return 0;
    }
  };

  auto r = Solve();
  cout << index << " / " << vd.size() << endl;
  cout << versions_sum << endl;
  cout << r << endl;
  return 0;
}
