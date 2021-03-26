#include "common/stl/base.h"

#include <random>

namespace {
class SolutionJumpingOverWalls {
 public:
  // const vector<pair<int, int>> vd { {1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  const vector<pair<int, int>> vd{{1, 1}, {1, -1}, {-1, 1}, {-1, -1},
                                  {1, 0}, {0, 1},  {-1, 0}, {0, -1}};
  const int xsize = 7, ysize = 7, dsize = int(vd.size());

  vector<bool> direction_used;
  vector<vector<int>> vused;

  void Init() {
    direction_used.resize(dsize);
    fill(direction_used.begin(), direction_used.end(), false);
    vused.resize(xsize);
    fill(vused.begin(), vused.end(), vector<int>(ysize, 0));
  }

  bool Inside(int x, int y) const {
    return (x >= 0) && (x < xsize) && (y >= 0) && (y < ysize);
  }

  bool ValidToSet(int x, int y, int d) const {
    if (vused[x][y] != 0) return false;
    for (int di = 0; di < dsize; ++di) {
      if (di == d) continue;
      for (int m = 1;; ++m) {
        int xnew = x - vd[di].first * m, ynew = y - vd[di].second * m;
        if (!Inside(xnew, ynew)) break;
        if (vused[xnew][ynew] != 0) return false;
      }
    }
    return true;
  }

  void PrintSolution() const {
    cout << xsize << " " << ysize << endl;
    for (int x = 0; x < xsize; ++x) {
      for (int y = 0; y < ysize; ++y) {
        int u = vused[x][y];
        cout << ((u == 0) ? '#'
                          : (u == 1) ? 'A' : (u == dsize + 1) ? 'B' : '.');
      }
      cout << endl;
    }
  }

  bool SolveR(int x, int y) {
    if (vused[x][y] == dsize + 1) {
      PrintSolution();
      return true;
    }
    vector<int> vda;
    for (int i = 0; i < dsize; ++i) {
      if (!direction_used[i]) vda.push_back(i);
    }
    assert(vda.size() > 0);
    random_shuffle(vda.begin(), vda.end());
    for (int di : vda) {
      direction_used[di] = true;
      vector<pair<int, int>> vnext;
      for (int m = 1;; ++m) {
        int xnew = x + vd[di].first * m, ynew = y + vd[di].second * m;
        if (!Inside(xnew, ynew)) break;
        if (ValidToSet(xnew, ynew, di)) vnext.push_back(make_pair(xnew, ynew));
      }
      random_shuffle(vnext.begin(), vnext.end());
      for (auto p : vnext) {
        vused[p.first][p.second] = vused[x][y] + 1;
        if (SolveR(p.first, p.second)) return true;
        vused[p.first][p.second] = 0;
      }
      direction_used[di] = false;
    }
    return false;
  }

  void Solve() {
    Init();
    vector<pair<int, int>> v;
    for (int x = 0; x < xsize * ysize; ++x)
      v.push_back(make_pair(x / ysize, x % ysize));
    random_shuffle(v.begin(), v.end());
    for (auto p : v) {
      vused[p.first][p.second] = 1;
      if (SolveR(p.first, p.second)) return;
      vused[p.first][p.second] = 0;
    }
    cout << "Solution not found" << endl;
  }
};
}  // namespace

int main_jumping_over_walls() {
  SolutionJumpingOverWalls s;
  s.Solve();
  return 0;
}
