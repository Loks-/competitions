// https://www.hackerrank.com/challenges/dice-stats

#include "common/statistics/moment2.h"
#include "common/stl/base.h"

#include <iomanip>
#include <numeric>

using TStat = stat::Moment2<>;

int main_dice_stats() {
  vector<double> vp(6), vi(6);
  double exp_approx = 0.;
  for (unsigned i = 0; i < 6; ++i) {
    vi[i] = 1.0 + i;
    cin >> vp[i];
    exp_approx += vi[i] * vp[i];
  }
  for (double& di : vi) di -= exp_approx;
  unsigned N;
  cin >> N;
  vector<TStat> vcurrent(6), vnew(6);
  for (unsigned iN = 0; iN < N; ++iN) {
    for (unsigned i = 0; i < 6; ++i) {
      vnew[i].Clear();
      if (iN == 0) {
        vnew[i].AddSample(vi[i], vp[i]);
      } else {
        for (unsigned j = 0; j < 6; ++j) {
          if (j == i) continue;
          double p = vp[i] / (1.0 - vp[j]);
          vnew[i] += (vcurrent[j] + vi[i]) * p;
        }
      }
    }
    vcurrent.swap(vnew);
  }
  TStat f = accumulate(vcurrent.begin(), vcurrent.end(), TStat());
  cout << setprecision(6) << fixed << f.Mean() + exp_approx * N << endl
       << f.Variance() << endl;
  return 0;
}
