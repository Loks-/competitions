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
    Transitions(double x) { m[{}] = x; }
    Transitions(const vector<unsigned>& v, double x) { m[v] = x; }

    void Compress(double eps = 1e-12) {
      for (auto it = m.begin(); it != m.end();) {
        if (it->second < eps * m.size())
          it = m.erase(it);
        else
          ++it;
      }
    }

    double VSum() const {
      double s = 0;
      for (auto& p : m) s += p.second;
      return s;
    }

    Transitions& operator+=(const Transitions& r) {
      for (auto& p : r.m) m[p.first] += p.second;
      return *this;
    }

    Transitions& operator*=(double x) {
      for (auto& p : m) p.second *= x;
      return *this;
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

  std::function<double(const vector<unsigned>&)> Get;

  auto Calc = [&](const vector<unsigned>& v) {
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
    return exp / (1 - p0);
  };

  Get = [&](const vector<unsigned>& v) -> double {
    auto it = mexp.find(v);
    if (it != mexp.end()) return it->second;
    auto expf = Calc(v);
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
    // One loop
    Init0(l);
    if (vsplit.size() <= l) vsplit.resize(l + 1);
    vsplit[l] = vsplit0[l];
    Transitions best_transition = vsplit0[l];
    double base_value = Calc({l}), best_value = base_value;
    // Multiple loops
    {
      vector<Transitions> vt0, vt1;
      for (unsigned k = 2; k < l / 2; ++k) {
        auto l0 = l / k, l1 = l0 + 1;
        auto m1 = l - (l0 * k), m0 = k - m1;
        vector<unsigned> vk(m0, l0);
        vk.insert(vk.end(), m1, l1);
        vt0.clear();
        vt0.push_back(Transitions(1.));
        for (auto u : vk) {
          vt1.clear();
          vt1.resize(vt0.size() + u);
          for (unsigned i = 1; i <= u; ++i) {
            for (unsigned j = 0; j < vt0.size(); ++j) {
              vt1[i + j] += vt0[j] * vsplit0[u - i];
            }
          }
          for (auto& t : vt1) t *= 1.0 / double(u);
          vt0.swap(vt1);
        }
        Transitions tcurrent;
        for (unsigned j = k; j < vt0.size(); ++j)
          tcurrent += vt0[j] * Transitions({j}, 1.);
        tcurrent.Compress();
        vsplit[l] = tcurrent;
        auto v = Calc({l});
        if (v < best_value) {
          best_value = v;
          best_transition = tcurrent;
          cout << "New best value: [" << base_value << "\t->" << best_value
               << "]\t{" << k << "}" << endl;
        } else {
          break;
        }
      }
    }
    // External loop length 2
    // double value_ell2 = base_value;
    if (l > 3) {
      Transitions tcurrent;
      auto l0 = l / 2 - 1, l1 = l - l0 - 2;
      for (unsigned i0 = 1; i0 <= l0; ++i0) {
        Transitions t00 = vsplit0[l0 - i0] * (1.0 / l0);
        Transitions t01 = t00 * Transitions({i0 + 1}, 1.0);
        for (unsigned i1 = 1; i1 <= l1; ++i1) {
          Transitions t10 = vsplit0[l1 - i1] * (1.0 / l1);
          Transitions t11 = t10 * Transitions({i1 + 1}, 1.0);
          tcurrent += t01 * t11;
          tcurrent += t00 * t10 * Transitions({i0 + i1 + 2}, 1.0);
        }
      }
      tcurrent *= .5;
      vsplit[l] = tcurrent;
      auto v = Calc({l});
      // value_ell2 = v;
      if (v < best_value) {
        best_value = v;
        best_transition = tcurrent;
        cout << "New best value: [" << base_value << "\t->" << best_value
             << "]\t{E2}" << endl;
      }
    }
    cout << "[" << l << "]\t-> " << best_value << "\t" << base_value - 1
         << "\t[" << mexp.size() << "]" << endl;
    vsplit[l] = best_transition;
    mexp[{l}] = best_value;
  };

  for (unsigned i = 2; i <= L; ++i) Init1(i);
  cout << "MExp size = " << mexp.size() << endl;

  return 0;
}
