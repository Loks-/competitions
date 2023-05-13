#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>
#include <unordered_set>

int main_colliding_encoding() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    auto vd = nvector::Read<uint64_t>(26);
    cin >> N;
    auto vs = nvector::Read<std::string>(N);
    std::unordered_set<uint64_t> sv;
    for (const auto& s : vs) {
      uint64_t v = 1;
      for (auto c : s) v = 10 * v + vd[c - 'A'];
      sv.insert(v);
    }
    cout << "Case #" << it << ": " << ((sv.size() == N) ? "NO" : "YES")
         << std::endl;
  }
  return 0;
}
