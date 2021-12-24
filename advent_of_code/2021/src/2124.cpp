#include "common/numeric/utils/abs.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2124() {
  class Linear {
   public:
    int64_t v0;
    vector<pair<unsigned, int64_t>> vp;

    Linear(int64_t x = 0) : v0(x) {}

    bool Constant() const { return vp.empty(); }

    void Compess() {
      for (unsigned i = 0; i < vp.size(); ++i) {
        if (vp[i].second == 0) vp.erase(vp.begin() + i--);
      }
    }

    void Set(int64_t x) {
      v0 = x;
      vp.clear();
    }

    void SetInd(unsigned ind) {
      Set(0);
      vp.push_back({ind, 1});
    }

    int64_t MinValue() const {
      int64_t x = v0;
      for (auto& p : vp) x += p.second * ((p.second > 0) ? 1 : 9);
      return x;
    }

    int64_t MaxValue() const {
      int64_t x = v0;
      for (auto& p : vp) x += p.second * ((p.second > 0) ? 9 : 1);
      return x;
    }

    void operator+=(const Linear& r) {
      v0 += r.v0;
      vector<pair<unsigned, int64_t>> vpnew;
      unsigned i = 0, j = 0;
      for (; (i < vp.size()) && (j < r.vp.size());) {
        if (vp[i].first < r.vp[j].first) {
          vpnew.push_back(vp[i++]);
        } else if (vp[i].first > r.vp[j].first) {
          vpnew.push_back(r.vp[j++]);
        } else {
          vpnew.push_back(vp[i++]);
          vpnew.back().second += vp[j++].second;
        }
      }
      for (; i < vp.size();) vpnew.push_back(vp[i++]);
      for (; j < r.vp.size();) vpnew.push_back(r.vp[j++]);
      vp.swap(vpnew);
      Compess();
    }

    void operator*=(const Linear& r) {
      assert(Constant() || r.Constant());
      if (r.Constant()) {
        for (auto& p : vp) p.second *= r.v0;
      } else if (Constant()) {
        vp = r.vp;
        for (auto& p : vp) p.second *= v0;
      }
      v0 *= r.v0;
      Compess();
    }

    void operator/=(const Linear& r) {
      assert(r.Constant());
      for (auto& p : vp) p.second /= r.v0;
      v0 /= r.v0;
    }

    void operator%=(const Linear& r) {
      assert(r.Constant());
      for (auto& p : vp) p.second %= r.v0;
      v0 %= r.v0;
      Compess();
    }

    bool Eql(const Linear& r) {
      if ((v0 == r.v0) && (vp == r.vp)) {
        Set(1);
      } else {
        auto x1 = MinValue(), x2 = MaxValue(), y1 = r.MinValue(),
             y2 = r.MaxValue();
        if ((x1 <= y2) && (y1 <= x2)) return false;
        Set(0);
      }
      return true;
    }

    void Print() const {
      cout << "\t[" << v0;
      for (auto& p : vp) cout << " + " << p.second << "x" << p.first;
      cout << "]";
    }
  };

  unordered_map<string, unsigned> m;
  m["x"] = 0;
  m["y"] = 1;
  m["z"] = 2;
  m["w"] = 3;

  auto Get = [&](const auto& vx, const string& s) -> Linear {
    auto it = m.find(s);
    if (it == m.end()) return Linear(stoi(s));
    return vx[it->second];
  };

  vector<Linear> vx(4, 0);
  unsigned l = 0;
  auto vs = nvector::ReadLines();
  vector<pair<Linear, Linear>> veq;
  for (auto& s : vs) {
    auto ss = Split(s, ' ');
    auto& x = vx[m[ss[1]]];
    if (ss[0] == "inp") {
      x.SetInd(l++);
      continue;
    }
    auto r = Get(vx, ss[2]);
    if (ss[0] == "add") {
      x += r;
    } else if (ss[0] == "mul") {
      x *= r;
    } else if (ss[0] == "div") {
      x /= r;
    } else if (ss[0] == "mod") {
      x %= r;
    } else if (ss[0] == "eql") {
      if (!x.Eql(r)) {
        veq.push_back({x, r});
        x.Set(1);
      }
    }
  }

  for (auto ab : {0, 1}) {
    vector<unsigned> vd(l, ab ? 1 : 9);
    for (auto p : veq) {
      assert((p.first.vp.size() == 1) && (p.second.vp.size() == 1));
      unsigned i0 = p.first.vp[0].first, i1 = p.second.vp[0].first;
      auto d = p.first.v0 - p.second.v0;
      if (ab) {
        if (d >= 0) {
          vd[i0] = 1;
          vd[i1] = 1 + d;
        } else {
          vd[i0] = 1 - d;
          vd[i1] = 1;
        }
      } else {
        if (d >= 0) {
          vd[i0] = 9 - d;
          vd[i1] = 9;
        } else {
          vd[i0] = 9;
          vd[i1] = 9 + d;
        }
      }
    }
    for (auto d : vd) cout << d;
    cout << endl;
  }
  return 0;
}
