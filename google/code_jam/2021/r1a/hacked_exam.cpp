#include "common/numeric/long/unsigned_gcd.h"
#include "common/numeric/long/unsigned_io.h"
#include "common/numeric/long/unsigned_multiplication.h"
#include "common/numeric/long/utils/binomial_coefficient.h"
#include "common/stl/base.h"
#include "common/stl/pair_io.h"
#include "common/vector/read.h"

#include <functional>
#include <string>

int main_hacked_exam() {
  unsigned T, N, Q, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> Q;
    auto v = nvector::Read<pair<string, unsigned>>(N);
    vector<unsigned> vm(Q);
    for (unsigned i = 0; i < Q; ++i) {
      unsigned m = 0, f = 1;
      for (unsigned j = 1; j < N; ++j) {
        if (v[j].first[i] != v[0].first[i]) m += f;
        f *= 2;
      }
      vm[i] = m;
    }
    K = 1u << (N - 1);
    vector<unsigned> vc(K, 0);
    for (auto m : vm) ++vc[m];

    vector<vector<unsigned>> vall;

    std::function<void(vector<unsigned>&, vector<unsigned>&, unsigned,
                       unsigned)>
        FindPossible = [&](vector<unsigned>& lvt, vector<unsigned>& lvc,
                           unsigned i, unsigned l) -> void {
      // Check for earlier stops
      for (unsigned j = 0; j < N; ++j) {
        if ((lvc[j] > v[j].second) || (lvc[j] + l < v[j].second)) return;
      }
      if (i == K) {
        // Found!
        vall.push_back(lvt);
        return;
      }
      for (unsigned k = 0; k <= vc[i]; ++k) {
        lvt[i] = k;
        lvc[0] += k;
        for (unsigned j = 1; j < N; ++j) {
          lvc[j] += ((1 << (j - 1)) & i) ? vc[i] - k : k;
        }
        FindPossible(lvt, lvc, i + 1, l - vc[i]);
        lvc[0] -= k;
        for (unsigned j = 1; j < N; ++j) {
          lvc[j] -= ((1 << (j - 1)) & i) ? vc[i] - k : k;
        }
      }
    };

    vector<unsigned> temp0(K, 0), temp1(N, 0);
    FindPossible(temp0, temp1, 0, Q);

    LongUnsigned total(0);
    vector<LongUnsigned> vcount;
    for (const auto& va : vall) {
      LongUnsigned r(1);
      for (unsigned i = 0; i < K; ++i)
        r *= GetBinomialCoefficientL(vc[i], va[i]);
      vcount.push_back(r);
      total += r;
    }

    assert(vall.size() > 0);
    unsigned best_index = 0;
    LongUnsigned best_score(0);
    for (unsigned index = 0; index < (1u << K); ++index) {
      LongUnsigned score(0);
      for (unsigned ia = 0; ia < vall.size(); ++ia) {
        const auto& va = vall[ia];
        unsigned correct_answers = 0;
        for (unsigned i = 0; i < K; ++i) {
          correct_answers += (index & (1u << i)) ? va[i] : vc[i] - va[i];
        }
        score += vcount[ia] * correct_answers;
      }
      if (best_score < score) {
        best_score = score;
        best_index = index;
      }
    }
    auto ld = GCD(best_score, total);

    cout << "Case #" << it << ": ";
    for (unsigned q = 0; q < Q; ++q) {
      unsigned m = vm[q];
      cout << char((best_index & (1u << m)) ? v[0].first[q]
                                            : 'T' + 'F' - v[0].first[q]);
    }
    cout << " " << best_score / ld << "/" << total / ld << endl;
  }
  return 0;
}
