#include "common/stl/full.h"
#include "common/stl/pair_io.h"
#include "common/string/utils/split.h"
#include "common/vector/read.h"
#include "common/vector/read_all.h"
#include "common/vector/read_from_line.h"
#include "common/vector/read_lines.h"
#include "common/vector/transform.h"
#include "common/vector/write.h"

int main_2110b() {
  // auto v = nvector::ReadAll<int64_t>();
  //   auto v = nvector::ReadFromLine<int64_t>(',');
  auto vs = nvector::ReadLines();
  string open = "([{<", close = ")]}>";
  auto match = [&](char c1, char c2) {
    auto i1 = find(open.begin(), open.end(), c1) - open.begin();
    auto i2 = find(close.begin(), close.end(), c2) - close.begin();
    return i1 == i2;
  };

  vector<int64_t> v{3, 57, 1197, 25137};
  vector<int64_t> v2;
  // int64_t r = 0;
  for (auto& s : vs) {
    string snew;
    bool bad = false;
    for (char c : s) {
      if (find(open.begin(), open.end(), c) != open.end()) {
        snew += c;
      } else if (!snew.empty() && match(snew.back(), c)) {
        snew.pop_back();
      } else {
        bad = true;
        break;
      }
    }
    if (!bad) {
      reverse(snew.begin(), snew.end());
      int64_t x = 0;
      for (char c : snew) {
        unsigned i = find(open.begin(), open.end(), c) - open.begin();
        x = 5 * x + i + 1;
      }
      v2.push_back(x);
    }
  }
  sort(v2.begin(), v2.end());
  cout << v2[v2.size() / 2] << endl;
  return 0;
}
