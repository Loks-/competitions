#pragma once

#include <vector>

template <class TModular>
class ModularFibonacci {
 protected:
  std::vector<TModular> vf;

 public:
  ModularFibonacci() : vf({0, 1}) {}

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
