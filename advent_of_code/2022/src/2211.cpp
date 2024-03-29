#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/extract_int.h"
#include "common/vector/read_lines.h"
#include "common/vector/split_empty_string.h"

int main_2211() {
  class Monkey {
   public:
    uint64_t count;
    vector<int64_t> items;
    bool mult;
    int64_t v2;
    int div;
    unsigned j1, j2;
  };

  auto vs = nvector::ReadLines();
  auto vvs = nvector::SplitEmptyString(vs);
  int64_t M = 1;
  vector<Monkey> vm0(vvs.size());
  for (unsigned j = 0; j < vvs.size(); ++j) {
    auto& v = vvs[j];
    vm0[j].items = nvector::ExtractInt<int64_t>(v[1], ", ");
    auto ss2 = Split(v[2], " ");
    vm0[j].mult = ss2[4] == "*";
    vm0[j].v2 = (ss2[5] == "old") ? 0 : stoi(ss2[5]);
    vm0[j].div = nvector::ExtractInt<int>(v[3])[0];
    vm0[j].j1 = nvector::ExtractInt<unsigned>(v[4])[0];
    vm0[j].j2 = nvector::ExtractInt<unsigned>(v[5])[0];
    M *= vm0[j].div;
  }

  for (bool ab : {false, true}) {
    auto vm = vm0;
    for (unsigned r = 0; r < (ab ? 10000 : 20); ++r) {
      for (unsigned j = 0; j < vm.size(); ++j) {
        auto& m = vm[j];
        for (auto x : m.items) {
          auto v = m.v2 ? m.v2 : x;
          x = (m.mult ? x * v : x + v);
          x = (ab ? x % M : x / 3);
          vm[((x % m.div) == 0) ? m.j1 : m.j2].items.push_back(x);
        }
        m.count += m.items.size();
        m.items.clear();
      }
    }
    sort(vm.begin(), vm.end(),
         [](auto& l, auto& r) { return l.count > r.count; });
    cout << vm[0].count * vm[1].count << endl;
  }
  return 0;
}
