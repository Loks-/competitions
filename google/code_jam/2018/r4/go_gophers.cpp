#include "common/stl/base.h"
#include "common/vector/read.h"

#include <numeric>

namespace {
struct Gopher {
  unsigned taste_min, taste_max, count;
};

class KGophers {
 public:
  unsigned count;
  bool valid;
  vector<Gopher> gophers;

  KGophers(unsigned k) : count(k), valid(true), gophers(1, {1, 1000000, k}) {}

  unsigned Q() const {
    if (!valid) return 0;
    for (const Gopher& g : gophers) {
      if ((g.count > 1) && (g.taste_max - g.taste_min > 0))
        return (g.taste_min + g.taste_max) / 2;
    }
    return 0;
  }

  void AddBlockI(unsigned q, const vector<unsigned>& v, unsigned b) {
    if (!valid) return;
    unsigned s = accumulate(v.begin() + b, v.begin() + b + count, 0u),
             k = count;
    // cerr << "AddBlockI\t" << count << "\t" << q << "\t" << b << "\t" << s
    //      << endl;
    for (unsigned i = 0; i < gophers.size(); ++i) {
      if (gophers[i].taste_max <= q)
        s -= gophers[i].count;
      else if (gophers[i].taste_min <= q)
        k = i;
    }
    if (k < count) {
      Gopher& g = gophers[k];
      if (s > g.count) {
        valid = false;
        cerr << "\tInvalidate1\n\t\t" << g.count << "\t" << s << endl;
      } else if (s == 0) {
        g.taste_min = q + 1;
      } else if (s == g.count) {
        g.taste_max = q;
      } else {
        Gopher gnew{q + 1, g.taste_max, g.count - s};
        g.taste_max = q;
        g.count = s;
        gophers.push_back(gnew);
        cerr << "\tSplit gophers" << endl;
      }
    } else {
      valid = (s == 0);
      if (s) cerr << "\tInvalidate2\t" << int(s) << endl;
    }
  }

  void AddBlock(unsigned q, unsigned s, const vector<unsigned>& v) {
    if (valid) cerr << "AddBlock\t" << count << "\t" << q << "\t" << s << endl;
    s %= count;
    unsigned b = (s ? count - s : 0);
    for (; b + count <= v.size(); b += count) AddBlockI(q, v, b);
  }
};

class UGophers {
 public:
  vector<KGophers> candidates;

  UGophers() {
    for (unsigned i = 2; i <= 25; ++i) candidates.push_back(KGophers(i));
  }

  bool Solved() const {
    unsigned s = 0;
    for (const KGophers& kg : candidates) s += (kg.valid ? 1 : 0);
    cerr << "Solved().s = " << s << endl;
    return s == 1;
  }

  unsigned Guess() const {
    for (const KGophers& kg : candidates) {
      if (kg.valid) return kg.count;
    }
    return 0;
  }

  unsigned Q() const {
    for (const KGophers& kg : candidates) {
      unsigned q = kg.Q();
      if (q) return q;
    }
    return 0;
  }

  void AddBlock(unsigned q, unsigned s, const vector<unsigned>& v) {
    for (KGophers& kg : candidates) kg.AddBlock(q, s, v);
  }
};

}  // namespace

int main_go_gophers() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    int S;
    cin >> S;
    if (S < 0) return S;
    UGophers gophers;
    unsigned s = 0, block_size = 50;
    vector<unsigned> vb(block_size);
    for (; !gophers.Solved(); s += block_size) {
      unsigned q = gophers.Q();
      if (!q) break;
      for (unsigned i = 0; i < block_size; ++i) cout << q << "\n";
      cout << flush;
      for (unsigned i = 0; i < block_size; ++i) cin >> vb[i];
      gophers.AddBlock(q, s, vb);
    }
    unsigned r = gophers.Guess();
    cerr << "Guess\t" << r << endl;
    cout << "-" << r << endl;
  }
  return 0;
}
