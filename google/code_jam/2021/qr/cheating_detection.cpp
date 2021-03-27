#include "common/optimization/functions/sigmoid.h"
#include "common/optimization/ternary_search.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>

int main_cheating_detection() {
  unsigned T, P = 100, Q = 10000, U, O = 5;
  double b = 3.0, eps = 1e-2;
  cin >> T >> U;
  for (unsigned it = 0; it < T; ++it) {
    auto vs = ReadVector<string>(P);
    std::vector<double> vp(P, 0.0), vq(Q, 0.0);

    auto FPQ = [&](double lp, double lq, char r) {
      return -opt::functions::LogSigmoid((r == '1' ? lp - lq : lq - lp));
    };

    auto FPQC = [&](double lp, double lq, char r) {
      return log(2) - ((r == '1') ? log(2 + exp(lq - lp)) : 0) + FPQ(lp, lq, r);
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

    // auto F = [&]() {
    //   double s = 0;
    //   for (unsigned p = 0; p < P; ++p) s += FP(p, vp[p]);
    //   return s;
    // };

    // cout << F() << endl;
    for (unsigned o = 0; o < O; ++o) {
      for (unsigned p = 0; p < P; ++p) {
        auto fp = [&](double x) { return FP(p, x); };
        vp[p] = opt::TernarySearch(fp, -b, b, eps);
      }
      //   cout << F() << endl;
      for (unsigned q = 0; q < Q; ++q) {
        auto fq = [&](double x) { return FQ(q, x); };
        vq[q] = opt::TernarySearch(fq, -b, b, eps);
      }
      //   cout << F() << endl;
    }

    double gain = -1000.0;
    unsigned c = 0;
    for (unsigned p = 0; p < P; ++p) {
      auto fpc = [&](double x) { return FPC(p, x); };
      double s = opt::TernarySearch(fpc, -b, b, eps);
      double g = FP(p, vp[p]) - FPC(p, s);
      if (gain < g) {
        gain = g;
        c = p;
      }
    }
    // cout << c << " " << gain << endl;

    cout << "Case #" << it + 1 << ": " << c + 1 << endl;
  }
  return 0;
}
