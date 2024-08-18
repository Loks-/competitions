#include "common/hash.h"
#include "common/hash/vector.h"
#include "common/stl/base.h"
#include "common/vector/enumerate.h"
#include "common/vector/mask.h"

#include <unordered_map>

namespace {
static const unsigned N = 15;

class CompactState {
 public:
  unsigned l;
  uint64_t mask;
  vector<unsigned> vc;

  CompactState() {
    l = N;
    mask = (1ull << N) - 1;
    vc.resize(l, 0);
  }

  bool operator==(const CompactState& r) const {
    return (mask == r.mask) && (vc == r.vc);
  }
};
}  // namespace

namespace nhash {
template <class Policy>
class Hash<Policy, CompactState> {
 public:
  constexpr size_t operator()(const CompactState& s) const {
    return HashCombine(Hash<Policy, vector<unsigned>>{}(s.vc), s.mask);
  }
};
}  // namespace nhash

namespace {
class Solver {
 public:
  unordered_map<CompactState, pair<double, unsigned>, DHash<CompactState>> m;

  uint64_t Count(const CompactState& s) {
    uint64_t r = 1;
    auto v = nvector::MaskToVector(s.mask);
    assert(v.size() == s.l);
    unsigned k = s.l;
    for (unsigned i = 0; i < s.l; ++i) {
      for (; (k > 0) && (v[k - 1] >= s.vc[i]);) --k;
      r *= (s.l - i - k);
    }
    return r;
  }

  double P2(const CompactState& s) {
    assert(s.l == 2);
    unsigned ss = 0;
    for (unsigned i = 0; i < N; ++i) {
      if (s.mask & (1ull << i)) ss += i;
    }
    for (auto c : s.vc) ss -= c;
    return (ss >= N) ? 1.0 : 0.;
  }

  bool GaveUp(const CompactState& s) const {
    auto v = nvector::MaskToVector(s.mask);
    assert(v.size() == s.l);
    unsigned ss = v[s.l - 1] + v[s.l - 2] - s.vc[s.l - 1] - s.vc[s.l - 2];
    return ss < N;
  }

  double P(const CompactState& s) {
    assert(s.l >= 2);
    auto it = m.find(s);
    if (it != m.end()) return it->second.first;
    double p = 0;
    unsigned r = N;
    if (s.l == 2) {
      p = P2(s);
    } else if (!GaveUp(s)) {
      CompactState st(s);
      uint64_t sc = Count(st);
      for (unsigned i = 0; i < s.l; ++i) {
        if ((i > 0) && (s.vc[i - 1] == s.vc[i])) continue;
        unsigned k = 0, j = 0;
        for (; j < s.vc[i]; ++j) {
          if (s.mask & (1ull << j)) ++k;
        }
        for (;; ++j) {
          if (s.mask & (1ull << j)) break;
        }
        if ((i > 0) && (j >= s.vc[i - 1])) continue;
        unsigned c = s.vc[i];
        st.vc[i] = j + 1;
        uint64_t scn = Count(st);
        double q1 = double(scn) / double(sc);
        double pc = ((q1 > 0) ? q1 * P(st) : 0.);
        st.vc[i] = c;
        st.mask &= ~(1ull << j);
        st.l -= 1;
        st.vc.erase(st.vc.begin() + i);
        pc += (1 - q1) * P(st);
        st.vc.insert(st.vc.begin() + i, c);
        st.l += 1;
        st.mask |= (1ull << j);
        if (p < pc) {
          p = pc;
          r = i;
        }
      }
    }
    m[s] = make_pair(p, r);
    return p;
  }

 public:
  unsigned Request(const CompactState& s) {
    // if (s.l > (N + 3) / 2) return 0;  // We never output first elements
    if (s.l > 6) return 0;  // Trade off between performance and quality
    P(s);
    return m[s].second;
  }

  size_t MapSize() const { return m.size(); }
};

class FullState : public CompactState {
 public:
  vector<unsigned> vp;

  FullState() { vp = nvector::Enumerate<unsigned>(1, N + 1); }
};

class SolverProxy {
 protected:
  Solver& s;
  FullState fs;
  unsigned last_request;

 public:
  SolverProxy(Solver& _s) : s(_s), last_request(N) {}

  double P() { return s.P(fs); }

  unsigned Request() {
    last_request = s.Request(fs);
    return (last_request == N) ? 0 : fs.vp[last_request];
  }

  void Update(unsigned result) {
    if (last_request != N) {
      if (result) {
        fs.vc[last_request] += 1;
      } else {
        unsigned k = fs.vc[last_request];
        fs.l -= 1;
        assert(fs.mask & (1ull << k));
        fs.mask &= ~(1ull << k);
        fs.vc.erase(fs.vc.begin() + last_request);
        fs.vp.erase(fs.vp.begin() + last_request);
      }
    }
  }

  unsigned Get1() const { return fs.vp[fs.l - 2]; }
  unsigned Get2() const { return fs.vp[fs.l - 1]; }
};
}  // namespace

int main_pen_testing() {
  Solver s;
  unsigned T, t1, t2;
  cin >> T >> t1 >> t2;
  // cerr << s.P(CompactState()) * T << " " << s.MapSize() << endl;
  std::vector<SolverProxy> vs(T, SolverProxy(s));
  bool all_zero = false;
  for (unsigned it = 1; !all_zero; ++it) {
    all_zero = true;
    for (SolverProxy& s : vs) {
      unsigned u = s.Request();
      if (u) all_zero = false;
      cout << u << " ";
    }
    cout << endl;
    if (all_zero) {
      for (SolverProxy& s : vs) {
        cout << s.Get1() << " " << s.Get2() << " ";
      }
      cout << endl;
    } else {
      for (SolverProxy& s : vs) {
        cin >> t1;
        s.Update(t1);
      }
    }
  }
  return 0;
}
