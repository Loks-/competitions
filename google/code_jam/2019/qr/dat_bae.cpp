#include "common/stl/base.h"
#include "common/vector/write.h"

#include <string>

namespace {
class Block {
 public:
  unsigned l;
  unsigned b;

  bool UnknownPositions() const { return (0 < b) && (b < l); }

  bool SmallB() const { return 2 * b < l; }

  string GetTestLine() const {
    if (!UnknownPositions()) return string(l, '0');
    string s(l, ' ');
    if (SmallB()) {
      for (unsigned i = 0; i < l; ++i) s[i] = ((i / b) % 2) ? '1' : '0';
    } else {
      for (unsigned i = 0; i < l; ++i) s[i] = (i < l / 2) ? '0' : '1';
    }
    return s;
  }

  vector<Block> Process(const char*& p) const {
    if (!UnknownPositions()) {
      p += (l - b);
      return {*this};
    } else if (SmallB()) {
      vector<Block> v;
      unsigned i = 0, j = 0;
      for (; (i + 1) * b < l; ++i) {
        char c = (i % 2) ? '1' : '0';
        unsigned k = 0;
        for (; (k < b) && (j < l - b) && (p[j] == c); ++j) ++k;
        v.push_back({b, b - k});
      }
      unsigned ll = l - i * b, kl = l - b - j;
      v.push_back({ll, ll - kl});
      p += (l - b);
      return v;
    } else {
      unsigned l0 = l / 2, l1 = l - l0;
      unsigned k0 = 0, k1 = 0;
      for (unsigned i = 0; i < l - b; ++i) {
        if (*p++ == '0')
          ++k0;
        else
          ++k1;
      }
      return {{l0, l0 - k0}, {l1, l1 - k1}};
    }
  }
};
}  // namespace

int main_dat_bae() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned n, b, f;
    cin >> n >> b >> f;
    vector<Block> vc{{n, b}}, vn;
    for (;;) {
      bool finished = true;
      string s;
      for (const Block& l : vc) {
        finished = finished && !l.UnknownPositions();
        s += l.GetTestLine();
      }
      if (finished) break;
      cout << s << endl;
      cin >> s;
      const char* p = s.c_str();
      vn.clear();
      for (const Block& l : vc) {
        auto vt = l.Process(p);
        vn.insert(vn.end(), vt.begin(), vt.end());
      }
      vc.swap(vn);
    }
    vector<unsigned> vpos;
    unsigned shift = 0;
    for (const Block& b : vc) {
      for (unsigned i = 0; i < b.b; ++i) {
        vpos.push_back(shift + i);
      }
      shift += b.l;
    }
    WriteVector(vpos);
    cin >> f;
    if (f != 1) return 0;
  }
  return 0;
}
