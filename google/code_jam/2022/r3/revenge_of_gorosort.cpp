#include "common/numeric/utils/usqrt.h"
#include "common/permutation/permutation.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/write.h"

int main_revenge_of_gorosort() {
  unsigned T, N, K, R, L = 2;
  cin >> T >> N >> K;
  vector<unsigned> vc(N);
  for (unsigned it = 1; it <= T;) {
    auto vi = nvector::Read<unsigned>(N);
    permutation::Permutation p(vi, true);
    auto pc = p.Cycles();
    fill(vc.begin(), vc.end(), 0);
    unsigned g = 0;
    for (unsigned i = 0; i < pc.size(); ++i) {
      auto& pi = pc[i];
      auto l = (pi.size() >= L * L) ? USqrt(pi.size()) : size_t(1);
      for (unsigned j = 0; j < l; ++j) {
        ++g;
        for (unsigned k = j * pi.size() / l; k < (j + 1) * pi.size() / l; ++k)
          vc[pi[k]] = g;
      }
    }
    for (unsigned i = 0; i < N; ++i) {
      if (vc[i] == 0) vc[i] = ++g;
    }
    nvector::Write(vc);
    cin >> R;
    if (R == 1) ++it;
  }
  return 0;
}
