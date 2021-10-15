#pragma once

#include <vector>

namespace modular {
namespace mstatic {
template <class TModular>
class Fibonacci {
 protected:
  std::vector<TModular> vf;

 public:
  Fibonacci() : vf({0, 1}) {}
  Fibonacci(const TModular& v0, const TModular& v1) : vf({v0, v1}) {}

  void Adjust(unsigned n) {
    unsigned i = unsigned(vf.size());
    for (; i <= n; ++i) vf.push_back(vf[i - 2] + vf[i - 1]);
  }

  TModular Get(unsigned n) {
    Adjust(n);
    return vf[n];
  }

  TModular operator()(unsigned n) { return Get(n); }
};
}  // namespace mstatic
}  // namespace modular
