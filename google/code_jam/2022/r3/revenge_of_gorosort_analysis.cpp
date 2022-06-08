#include "common/assert_exception.h"
#include "common/stl/base.h"
#include "common/stl/hash/vector.h"
#include "common/timer.h"
#include "common/vector/munion.h"

#include <functional>
#include <unordered_map>

int main_revenge_of_gorosort_analysis() {
  class Transitions {
   public:
    unordered_map<vector<unsigned>, double> m;

    Transitions() {}

    Transitions(int x) {
      Assert(x == 1, "Only x = 1 is supported.");
      m[{}] = 1.0;
    }

    void Compress(double eps = 1e-12) {
      for (auto it = m.begin(); it != m.end();) {
        if (it->second < eps * m.size())
          it = m.erase(it);
        else
          ++it;
      }
    }

    Transitions operator*(const Transitions& r) const {
      Transitions t;
      for (auto& p1 : m) {
        for (auto& p2 : r.m) {
          // if (p1.second * p2.second < 1e-12) continue;
          t.m[nvector::MUnionV(p1.first, p2.first)] += p1.second * p2.second;
        }
      }
      // t.Compress();
      return t;
    }

    Transitions& operator*=(const Transitions& r) {
      auto t = (*this) * r;
      m.swap(t.m);
      return *this;
    }
  };

  unsigned L = 40;
  unordered_map<vector<unsigned>, double> mexp;
  vector<Transitions> vsplit0(2, Transitions(1)), vsplit = vsplit0;
  mexp[{}] = 0;

  std::function<double(const vector<unsigned>&)> Get =
      [&](const vector<unsigned>& v) -> double {
    auto it = mexp.find(v);
    if (it != mexp.end()) return it->second;
    Transitions t(1);
    for (auto u : v) t *= vsplit[u];
    double p0 = 0, exp = 1;
    for (auto& p : t.m) {
      if (p.first == v) {
        p0 += p.second;
      } else {
        exp += p.second * Get(p.first);
      }
    }
    auto expf = exp / (1 - p0);
    mexp[v] = expf;
    return expf;
  };

  auto Init0 = [&](unsigned l) {
    unordered_map<vector<unsigned>, double> mvc;
    vector<unsigned> v;
    for (unsigned i = 1; i <= l; ++i) {
      for (auto& p : vsplit0[l - i].m) {
        v = p.first;
        if (i > 1) {
          auto it = upper_bound(v.begin(), v.end(), i);
          v.insert(it, i);
        }
        mvc[v] += p.second / l;
      }
    }
    if (vsplit0.size() <= l) vsplit0.resize(l + 1);
    vsplit0[l].m.swap(mvc);
    vsplit0[l].Compress();
  };

  auto Init1 = [&](unsigned l) {
    // First version use one color for whole loop
    Init0(l);
    if (vsplit.size() <= l) vsplit.resize(l + 1);
    vsplit[l] = vsplit0[l];
    auto f = Get({l});
    cout << "[" << l << "]\t-> " << f << "\t[" << mexp.size() << "]" << endl;
  };

  for (unsigned i = 2; i <= L; ++i) Init1(i);
  cout << "MExp size = " << mexp.size() << endl;

  return 0;
}
