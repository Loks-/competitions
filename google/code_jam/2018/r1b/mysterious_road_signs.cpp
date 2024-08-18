#include "common/hash.h"
#include "common/hash/pair.h"
#include "common/stl/base.h"

#include <array>
#include <unordered_set>

// O(S log S) solution

namespace {
struct BestIntervals {
  unsigned max_length;
  unordered_set<pair<unsigned, unsigned>, DHash<pair<unsigned, unsigned>>>
      intervals;

  BestIntervals() : max_length(0) {}

  void AddInterval(unsigned first, unsigned last) {
    unsigned length = last - first;
    if (max_length < length) {
      max_length = length;
      intervals.clear();
    }
    if (length == max_length) intervals.insert(make_pair(first, last));
  }
};

class MysteriousRoadSign {
  unsigned S;
  array<vector<int64_t>, 2> targets;
  BestIntervals best_intervals;

  void SolveC(unsigned begin, unsigned center, unsigned end, int64_t value0,
              int64_t value1) {
    unsigned i0 = center, i1 = center;
    for (; i0 > begin; --i0) {
      if ((targets[0][i0 - 1] != value0) && (targets[1][i0 - 1] != value1))
        break;
    }
    for (; i1 < end; ++i1) {
      if ((targets[0][i1] != value0) && (targets[1][i1] != value1)) break;
    }
    best_intervals.AddInterval(i0, i1);
  }

  void SolveR(unsigned begin, unsigned end) {
    unsigned l = end - begin;
    if (l < best_intervals.max_length) return;
    if (l <= 2) {
      best_intervals.AddInterval(begin, end);
      return;
    }
    unsigned m = begin + l / 2;
    for (unsigned j = 0; j < 2; ++j) {
      int64_t value0 = targets[0][m], value1l = 0, value1r = 0;
      for (unsigned i = m + 1; i < end; ++i) {
        if (targets[0][i] != value0) {
          value1r = targets[1][i];
          break;
        }
      }
      for (unsigned i = m; i > begin; --i) {
        if (targets[0][i - 1] != value0) {
          value1l = targets[1][i - 1];
          break;
        }
      }
      SolveC(begin, m, end, value0, value1l);
      if (value1r != value1l) SolveC(begin, m, end, value0, value1r);
      targets[0].swap(targets[1]);
    }
    SolveR(begin, m);
    SolveR(m + 1, end);
  }

 public:
  void Solve(unsigned itest) {
    cin >> S;
    for (unsigned i = 0; i < S; ++i) {
      int64_t d, a, b;
      cin >> d >> a >> b;
      targets[0].push_back(d + a);
      targets[1].push_back(d - b);
    }
    SolveR(0, S);
    cout << "Case #" << itest << ": " << best_intervals.max_length << " "
         << best_intervals.intervals.size() << endl;
  }
};
}  // namespace

int main_mysterious_road_signs() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    MysteriousRoadSign s;
    s.Solve(it);
  }
  return 0;
}
