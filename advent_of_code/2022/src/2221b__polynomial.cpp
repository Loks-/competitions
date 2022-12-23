#include "common/numeric/long/fraction_io.h"
#include "common/polynomial/multiplication.h"
#include "common/polynomial/polynomial.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <queue>
#include <unordered_map>

int main_2221b__polynomial() {
  using TValue = polynomial::Polynomial<TLFraction>;

  class Monkey {
   public:
    string name;
    TValue value;
    string op;
    string v1, v2;
    unsigned p;
    vector<string> vu;
  };

  auto vs = nvector::ReadLines();
  unordered_map<string, Monkey> mm;
  queue<string> mq;
  for (auto& s : vs) {
    auto ss = Split(s, ": :");
    Monkey m;
    m.name = ss[0];
    if (ss.size() > 2) {
      m.v1 = ss[1];
      m.op = ss[2];
      m.v2 = ss[3];
      m.p = 2;
    } else if (m.name == "humn") {
      m.value = TValue(TLFraction(), MakeTLFraction(1));
      m.p = 0;
      mq.push(m.name);
    } else {
      m.value = TValue(MakeTLFraction(stoi(ss[1])));
      m.p = 0;
      mq.push(m.name);
    }
    mm[m.name] = m;
  }
  for (auto mp : mm) {
    if (!mp.second.op.empty()) {
      mm[mp.second.v1].vu.push_back(mp.second.name);
      mm[mp.second.v2].vu.push_back(mp.second.name);
    }
  }
  mm["root"].op = "-";

  for (; !mq.empty(); mq.pop()) {
    auto mn = mq.front();
    auto& m = mm[mn];
    if (!m.op.empty()) {
      auto v1 = mm[m.v1].value, v2 = mm[m.v2].value;
      if (m.op == "+") m.value = v1 + v2;
      if (m.op == "*") m.value = v1 * v2;
      if (m.op == "-") m.value = v1 - v2;
      if (m.op == "/") m.value = v1 / v2[0];
    }

    for (auto mn2 : m.vu) {
      auto& m2 = mm[mn2];
      if (--m2.p == 0) mq.push(mn2);
    }
  }
  auto p = mm["root"].value;
  cout << (-p[0] / p[1]).GetN() << endl;
  return 0;
}
