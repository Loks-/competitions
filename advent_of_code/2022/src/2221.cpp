#include "common/numeric/long/fraction_io.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <queue>
#include <unordered_map>

int main_2221() {
  class Monkey {
   public:
    string name;
    TLFraction value;
    string op;
    string v1, v2;
    unsigned p;
    vector<string> vu;
  };

  auto vs = nvector::ReadLines();
  unordered_map<string, Monkey> mm;
  for (auto& s : vs) {
    auto ss = Split(s, ": :");
    Monkey m;
    m.name = ss[0];
    if (ss.size() > 2) {
      m.v1 = ss[1];
      m.op = ss[2];
      m.v2 = ss[3];
    } else {
      m.value = MakeTLFraction(stoi(ss[1]));
    }
    mm[m.name] = m;
  }
  for (auto mp : mm) {
    if (!mp.second.op.empty()) {
      mm[mp.second.v1].vu.push_back(mp.second.name);
      mm[mp.second.v2].vu.push_back(mp.second.name);
    }
  }

  auto Eval = [&](TLFraction x) {
    mm["humn"].value = x;
    queue<string> mq;
    for (auto& it : mm) {
      auto& m = it.second;
      if (m.op.empty()) {
        mq.push(m.name);
      } else {
        m.p = 2;
      }
    }
    for (; !mq.empty(); mq.pop()) {
      auto mn = mq.front();
      auto& m = mm[mn];
      if (!m.op.empty()) {
        auto v1 = mm[m.v1].value, v2 = mm[m.v2].value;
        if (m.op == "+") m.value = v1 + v2;
        if (m.op == "*") m.value = v1 * v2;
        if (m.op == "-") m.value = v1 - v2;
        if (m.op == "/") m.value = v1 / v2;
      }
      for (auto mn2 : m.vu) {
        auto& m2 = mm[mn2];
        if (--m2.p == 0) mq.push(mn2);
      }
    }
    return mm["root"].value;
  };

  cout << Eval(mm["humn"].value).GetN() << endl;
  mm["root"].op = "-";
  auto f0 = Eval(MakeTLFraction(0)), f1 = Eval(MakeTLFraction(1));
  cout << (-f0 / (f1 - f0)).GetN() << endl;
  return 0;
}
