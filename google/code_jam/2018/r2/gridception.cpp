#include "common/data_structures/disjoint_set.h"
#include "common/stl/base.h"

namespace {
unsigned MaxSize(ds::DisjointSet& djs) {
  auto vr = djs.GetRepresentatives();
  unsigned msize = 1;
  for (unsigned r : vr) msize = max(msize, djs.GetSize(r));
  return msize;
}

unsigned CheckB(const vector<string>& v, unsigned R, unsigned C) {
  ds::DisjointSet djs(R * C);
  for (unsigned i = 0; i < R; ++i) {
    for (unsigned j = 0; j < C; ++j) {
      if (v[i][j] != 'B') continue;
      if ((i + 1 < R) && (v[i + 1][j] == 'B')) {
        djs.Union(i * C + j, (i + 1) * C + j);
      }
      if ((j + 1 < C) && (v[i][j + 1] == 'B')) {
        djs.Union(i * C + j, i * C + j + 1);
      }
    }
  }
  return MaxSize(djs);
}

void Invert(vector<string>& v, unsigned i0, unsigned i1, unsigned j0,
            unsigned j1) {
  for (unsigned i = i0; i < i1; ++i) {
    for (unsigned j = j0; j < j1; ++j) {
      v[i][j] = 'B' + 'W' - v[i][j];
    }
  }
}
}  // namespace

int main_gridception() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned R, C;
    cin >> R >> C;
    vector<string> v(R);
    for (unsigned i = 0; i < R; ++i) {
      cin >> v[i];
      assert(v[i].size() == C);
      for (unsigned j = 0; j < C; ++j) {
        assert((v[i][j] == 'B') || (v[i][j] == 'W'));
      }
    }
    unsigned best = 1;
    vector<bool> vcb(16, false);
    for (unsigned i = 0; i < R; ++i) {
      for (unsigned j = 0; j < C; ++j) {
        for (unsigned ih = 0; ih < 2; ++ih) {
          for (unsigned jh = 0; jh < 2; ++jh) {
            if (i + ih >= R) continue;
            if (j + jh >= C) continue;
            unsigned c = 0;
            for (unsigned k = 0; k < 4; ++k) {
              unsigned ik = i + (k & 1) * ih;
              unsigned jk = j + (k / 2) * jh;
              if (v[ik][jk] == 'W') c |= (1 << k);
            }
            vcb[c] = true;
          }
        }
      }
    }
    for (unsigned i = 0; i < R; ++i) {
      for (unsigned j = 0; j < C; ++j) {
        for (unsigned c = 0; c < 16; ++c) {
          if (!vcb[c]) continue;
          if (c & 1) Invert(v, 0, i, 0, j);
          if (c & 2) Invert(v, i, R, 0, j);
          if (c & 4) Invert(v, 0, i, j, C);
          if (c & 8) Invert(v, i, R, j, C);
          best = max(best, CheckB(v, R, C));
          if (c & 1) Invert(v, 0, i, 0, j);
          if (c & 2) Invert(v, i, R, 0, j);
          if (c & 4) Invert(v, 0, i, j, C);
          if (c & 8) Invert(v, i, R, j, C);
        }
      }
    }
    cout << "Case #" << it << ": " << best << endl;
  }
  return 0;
}
