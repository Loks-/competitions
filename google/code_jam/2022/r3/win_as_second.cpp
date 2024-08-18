#include "common/hash.h"
#include "common/hash/vector.h"
#include "common/stl/base.h"
#include "common/stl/pair.h"
#include "common/vector/first_missed.h"
#include "common/vector/read.h"

#include <unordered_map>

int main_win_as_second() {
  unsigned T, N, NMax = 41, M;
  vector<unsigned> vnim_path1(NMax, 0);
  for (unsigned i = 1; i < NMax; ++i) {
    vector<unsigned> vp;
    for (unsigned j = 1; j <= min(i, 3u); ++j) {
      for (unsigned k = 0; k <= i - j; ++k) {
        vp.push_back(vnim_path1[k] ^ vnim_path1[i - j - k]);
      }
    }
    vnim_path1[i] = nvector::FirstMissed(vp);
  }

  unordered_map<vector<unsigned>, unsigned, DHash<vector<unsigned>>> mnim_star;

  std::function<unsigned(const vector<unsigned>&)> GetNimStar =
      [&](const vector<unsigned>& v) -> unsigned {
    // Check cache
    if (v.size() == 0) return vnim_path1[1];
    if (v.size() == 1) return vnim_path1[v[0] + 1];
    if (v.size() == 2) return vnim_path1[v[0] + v[1] + 1];
    auto v1(v);
    sort(v1.begin(), v1.end());
    auto it = upper_bound(v1.begin(), v1.end(), 0);
    v1.erase(v1.begin(), it);
    if (v1.size() == 0) return vnim_path1[1];
    if (v1.size() == 1) return vnim_path1[v1[0] + 1];
    if (v1.size() == 2) return vnim_path1[v1[0] + v1[1] + 1];
    auto its = mnim_star.find(v1);
    if (its != mnim_star.end()) return its->second;
    // New value
    vector<unsigned> vp;
    for (unsigned vi = 0; vi < v1.size(); ++vi) {
      auto li = v1[vi];
      for (unsigned j = 0; j < li; ++j) {
        v1[vi] = j;
        unsigned p = GetNimStar(v1);
        for (unsigned k = 1; k <= min(3u, li - j); ++k)
          vp.push_back(p ^ vnim_path1[li - j - k]);
      }
      v1[vi] = li;
      if (li >= 2) {
        unsigned p = vnim_path1[li - 2] ^ vnim_path1[li];
        for (auto j : v1) p ^= vnim_path1[j];
        vp.push_back(p);
      }
    }
    for (unsigned i = 0; i < (1u << v1.size()); ++i) {
      unsigned p = 0;
      for (unsigned j = 0; j < v1.size(); ++j) {
        p ^= vnim_path1[v1[j] - ((i >> j) & 1)];
      }
      vp.push_back(p);
    }
    auto nimv = nvector::FirstMissed(vp);
    mnim_star[v1] = nimv;
    return nimv;
  };

  vector<vector<unsigned>> vstar_nim0(NMax);

  std::function<void(vector<unsigned>&, unsigned)> Search =
      [&](vector<unsigned>& v, unsigned l) -> void {
    auto nimv = GetNimStar(v);
    if ((nimv == 0) && (vstar_nim0[l].empty())) vstar_nim0[l] = v;
    if (v.size() >= 10) return;
    unsigned i2 = v.back();
    v.push_back(0);
    for (; i2 + l < NMax; ++i2) {
      v.back() = i2;
      Search(v, i2 + l);
    }
    v.pop_back();
  };

  vector<unsigned> vsearch(1);
  for (unsigned i1 = 1; 2 * i1 < NMax; ++i1) {
    vsearch[0] = i1;
    Search(vsearch, i1 + 1);
  }

  class Position {
   public:
    vector<unsigned> star;
    vector<unsigned> first_index;
    vector<pair<unsigned, unsigned>> paths;

    Position(const vector<unsigned>& inital_star) {
      star = inital_star;
      first_index.resize(star.size());
      for (unsigned i = 0; i < star.size(); ++i)
        first_index[i] = ((i > 0) ? first_index[i - 1] + star[i - 1] : 2u);
      paths.clear();
    }

    bool Finished() const { return star.empty() && paths.empty(); }

    void Color(unsigned x) {
      if (x == 1) {
        for (unsigned i = 0; i < star.size(); ++i) {
          if (star[i] > 0) paths.push_back({first_index[i], star[i]});
        }
        star.clear();
        sort(paths.begin(), paths.end());
        return;
      }
      if (!star.empty()) {
        auto it = upper_bound(first_index.begin(), first_index.end(), x);
        if (it != first_index.begin()) {
          unsigned ind = (it - first_index.begin()) - 1;
          if (x < first_index[ind] + star[ind]) {
            unsigned l = first_index[ind] + star[ind] - x - 1;
            if (l > 0) {
              auto p = make_pair(x + 1, l);
              auto it2 = lower_bound(paths.begin(), paths.end(), p);
              paths.insert(it2, p);
            }
            star[ind] = x - first_index[ind];
            return;
          }
        }
      }
      auto it2 = upper_bound(paths.begin(), paths.end(), make_pair(x + 1, 0u));
      assert(it2 != paths.begin());
      auto ind = it2 - paths.begin() - 1;
      auto pe = paths[ind].first + paths[ind].second;
      assert((x >= paths[ind].first) && (x < pe));
      if (x + 1 < pe) paths.insert(it2, make_pair(x + 1, pe - x - 1));
      if (x == paths[ind].first) {
        paths.erase(paths.begin() + ind);
      } else {
        paths[ind].second = x - paths[ind].first;
      }
    }

    void ColorCout(unsigned x) {
      Color(x);
      cout << x << " ";
    }

    void ReadMove() {
      unsigned l;
      cin >> l;
      auto v = nvector::Read<unsigned>(l);
      for (auto u : v) Color(u);
    }
  };

  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    const auto& star = vstar_nim0[N];
    for (unsigned i = 0, j = 2; i < star.size(); ++i) {
      for (unsigned k = 0; k < star[i]; ++k, ++j)
        cout << ((k > 0) ? j - 1 : 1u) << " " << j << endl;
    }
    cin >> M;
    for (unsigned im = 0; im < M; ++im) {
      Position p(star);
      for (; !p.Finished();) {
        p.ReadMove();
        // Find optimal move
        auto pstar = p.star;
        auto nim_star = (pstar.empty() ? 0u : GetNimStar(pstar));
        unsigned nim_paths = 0;
        for (auto& pp : p.paths) nim_paths ^= vnim_path1[pp.second];
        unsigned nim_current = nim_star ^ nim_paths;
        bool found = false;
        // Search paths
        for (auto pp : p.paths) {
          auto nim_req = nim_current ^ vnim_path1[pp.second];
          for (unsigned j = 1; j <= min(pp.second, 3u); ++j) {
            for (unsigned k = 0; k <= pp.second - j; ++k) {
              auto v = vnim_path1[k] ^ vnim_path1[pp.second - k - j];
              if (v == nim_req) {
                found = true;
                cout << j << endl;
                if (j > 1) p.ColorCout(pp.first + k + 1);
                if (j == 3) p.ColorCout(pp.first + k + 2);
                p.ColorCout(pp.first + k);
                cout << endl;
                break;
              }
            }
            if (found) break;
          }
          if (found) break;
        }
        // Search star
        if (!found) {
          for (unsigned vi = 0; vi < pstar.size(); ++vi) {
            auto li = pstar[vi];
            for (unsigned j = 0; j < li; ++j) {
              pstar[vi] = j;
              unsigned pnim = GetNimStar(pstar);
              for (unsigned k = 1; k <= min(3u, li - j); ++k) {
                if ((pnim ^ vnim_path1[li - j - k]) == nim_paths) {
                  found = true;
                  cout << k << endl;
                  if (k > 1) p.ColorCout(p.first_index[vi] + j + 1);
                  if (k == 3) p.ColorCout(p.first_index[vi] + j + 2);
                  p.ColorCout(p.first_index[vi] + j);
                  cout << endl;
                  break;
                }
              }
              if (found) break;
            }
            pstar[vi] = li;
            if (!found && (li >= 2)) {
              unsigned pnim = vnim_path1[li - 2] ^ vnim_path1[li];
              for (auto j : pstar) pnim ^= vnim_path1[j];
              if (pnim == nim_paths) {
                found = true;
                cout << 3 << endl;
                p.ColorCout(p.first_index[vi]);
                p.ColorCout(p.first_index[vi] + 1);
                p.ColorCout(1);
                cout << endl;
              }
            }
            if (found) break;
          }
        }
        if (!found) {
          for (unsigned i = 0; i < (1u << pstar.size()); ++i) {
            bool skip = false;
            unsigned l = 1;
            for (unsigned j = 0; j < pstar.size(); ++j) {
              auto b = ((i >> j) & 1);
              if (b > pstar[j]) skip = true;
              l += b;
            }
            if (skip) continue;
            unsigned pnim = 0;
            for (unsigned j = 0; j < pstar.size(); ++j)
              pnim ^= vnim_path1[pstar[j] - ((i >> j) & 1)];
            if (pnim == nim_paths) {
              found = true;
              cout << l << endl;
              p.ColorCout(1);
              for (unsigned j = 0; j < pstar.size(); ++j) {
                if ((i >> j) & 1) p.ColorCout(p.first_index[j]);
              }
              cout << endl;
              break;
            }
          }
        }
        assert(found);
      }
    }
  }

  return 0;
}
