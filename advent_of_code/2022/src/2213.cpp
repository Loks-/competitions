#include "common/files/json.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"
#include "common/vector/split_empty_string.h"

#include <functional>

int main_2213() {
  using Json = files::JSON;

  function<int(const Json& j1, const Json& j2)> JCmp =
      [&](const Json& j1, const Json& j2) -> int {
    if ((j1.Type() == Json::INTEGER) && (j2.Type() == Json::INTEGER))
      return j1.GetInteger() - j2.GetInteger();
    if (j1.Type() == Json::INTEGER) {
      Json t1;
      t1.SetArray(vector<Json>{j1});
      return JCmp(t1, j2);
    }
    if (j2.Type() == Json::INTEGER) return -JCmp(j2, j1);
    unsigned k = min(j1.Size(), j2.Size());
    for (unsigned i = 0; i < k; ++i) {
      auto x = JCmp(j1.GetValue(i), j2.GetValue(i));
      if (x) return x;
    }
    return int(j1.Size()) - int(j2.Size());
  };

  auto JCmpB = [&](const Json& j1, const Json& j2) { return JCmp(j1, j2) < 0; };

  auto vs = nvector::ReadLines();
  vector<Json> vj;
  for (auto& s : vs) {
    if (!s.empty()) {
      vj.push_back({});
      vj.back().Parse(s);
    }
  }
  unsigned jr1 = 0, jr2 = 1;
  for (unsigned i = 0; i < vj.size(); i += 2) {
    if (JCmpB(vj[i], vj[i + 1])) jr1 += i / 2 + 1;
  }
  vector<Json> vje(2);
  vje[0].Parse("[[2]]");
  vje[1].Parse("[[6]]");
  vj.insert(vj.end(), vje.begin(), vje.end());
  sort(vj.begin(), vj.end(), JCmpB);
  for (const auto& j : vje)
    jr2 *= upper_bound(vj.begin(), vj.end(), j, JCmpB) - vj.begin();
  cout << jr1 << endl << jr2 << endl;
  return 0;
}
