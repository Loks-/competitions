#include "common/assert_exception.h"
#include "common/numeric/factorial.h"
#include "common/permutation/permutation.h"
#include "common/stl/base.h"
#include "common/stl/hash/vector.h"
#include "common/stl/pair.h"
#include "common/vector/enumerate.h"
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

    Transitions operator*(const Transitions& r) const {
      Transitions t;
      for (auto& p1 : m) {
        for (auto& p2 : r.m) {
          t.m[nvector::MUnionV(p1.first, p2.first)] += p1.second * p2.second;
        }
      }
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
  vector<Transitions> vsplit0, vsplit;
  mexp[{}] = 0;

  auto Solve = [&](const vector<unsigned>& v) {
    assert(!v.empty());
    Transitions t(1);
    for (auto u : v) t *= vsplit[u];
    double p0 = 0, exp = 1;
    for (auto& p : t.m) {
      if (p.first == v) {
        p0 += p.second;
      } else {
        auto it = mexp.find(p.first);
        Assert(it != mexp.end());
        exp += p.second * it->second;
      }
    }
    auto expf = exp / (1 - p0);
    if (v.size() == 1) {
      cout << "[";
      for (auto u : v) cout << u << " ";
      // cout << "] -> (" << p0 << "\t" << exp << "\t" << expf << ")" << endl;
      cout << "]\t-> " << expf << endl;
    }
    mexp[v] = expf;
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
  };

  auto Init1 = [&](unsigned l) {
    // First version use one color for whole loop
    Init0(l);
    if (vsplit.size() <= l) vsplit.resize(l + 1);
    vsplit[l] = vsplit0[l];
  };

  vsplit0.resize(2, Transitions(1));
  vsplit = vsplit0;

  vector<pair<uint64_t, vector<unsigned>>> vallsplits;
  pair<uint64_t, vector<unsigned>> pvcurrent{0ull, {}};

  std::function<void(unsigned)> AddAll = [&](unsigned l) {
    vallsplits.push_back(pvcurrent);
    for (unsigned i = pvcurrent.second.back(); i <= l; ++i) {
      pvcurrent.first += i * i;
      pvcurrent.second.push_back(i);
      AddAll(l - i);
      pvcurrent.second.pop_back();
      pvcurrent.first -= i * i;
    }
  };

  pvcurrent.second.resize(1);
  for (unsigned i = 2; i <= L; ++i) {
    pvcurrent.first = i * i;
    pvcurrent.second[0] = i;
    AddAll(L - i);
  }
  sort(vallsplits.begin(), vallsplits.end());
  cout << "Total splits = " << vallsplits.size() << endl;

  for (auto& ps : vallsplits) {
    if (ps.second.size() == 1) Init1(ps.second[0]);
    Solve(ps.second);
  }

  //   for (unsigned i = 2; i <= L; ++i) Init1(i);
  return 0;
}
