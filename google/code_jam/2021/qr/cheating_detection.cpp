#include "common/optimization/approximation/table.h"
#include "common/optimization/functions/sigmoid.h"
#include "common/optimization/minimum.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>

int main_cheating_detection() {
  unsigned T, P = 100, Q = 10000, U, O = 2;
  double b = 3.0, eps = 0.5;

  double l2 = log(2);
  auto lc = [](double x) { return log(2 + exp(x)); };
  opt::approximation::Table<> lst(opt::functions::LogSigmoid, -2 * b, 2 * b,
                                  8 * b / eps);
  opt::approximation::Table<> lct(lc, -2 * b, 2 * b, 8 * b / eps);

  cin >> T >> U;
  for (unsigned it = 0; it < T; ++it) {
    auto vs = nvector::Read<string>(P);
    std::vector<double> vp(P, 0.0), vq(Q, 0.0);

    auto FPQ = [&](double lp, double lq, char r) {
      return -lst((r == '1' ? lp - lq : lq - lp));
    };

    auto FPQC = [&](double lp, double lq, char r) {
      return l2 - ((r == '1') ? lct(lq - lp) : 0) + FPQ(lp, lq, r);
    };

    auto FP = [&](unsigned p, double lp) {
      double s = 0;
      for (unsigned q = 0; q < Q; ++q) s += FPQ(lp, vq[q], vs[p][q]);
      return s;
    };

    auto FPC = [&](unsigned p, double lp) {
      double s = 0;
      for (unsigned q = 0; q < Q; ++q) s += FPQC(lp, vq[q], vs[p][q]);
      return s;
    };

    auto FQ = [&](unsigned q, double lq) {
      double s = 0;
      for (unsigned p = 0; p < P; ++p) s += FPQ(vp[p], lq, vs[p][q]);
      return s;
    };

    for (unsigned o = 0; o < O; ++o) {
      for (unsigned p = 0; p < P; ++p) {
        auto fp = [&](double x) { return FP(p, x); };
        vp[p] = opt::Minimum(fp, -b, b, eps);
      }
      for (unsigned q = 0; q < Q; ++q) {
        auto fq = [&](double x) { return FQ(q, x); };
        vq[q] = opt::Minimum(fq, -b, b, eps);
      }
    }

    double gain = -1000.0;
    unsigned c = 0;
    for (unsigned p = 0; p < P; ++p) {
      auto fpc = [&](double x) { return FPC(p, x); };
      double s = opt::Minimum(fpc, -b, b, eps);
      double g = FP(p, vp[p]) - FPC(p, s);
      if (gain < g) {
        gain = g;
        c = p;
      }
    }

    cout << "Case #" << it + 1 << ": " << c + 1 << endl;
  }
  return 0;
}
