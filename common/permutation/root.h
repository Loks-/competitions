#pragma once

#include "common/modular/arithmetic.h"
#include "common/numeric/utils/gcd.h"
#include "common/permutation/permutation.h"
#include "common/permutation/simple_cycle_pow.h"
#include <vector>

// If root is not exist function returns permutation with size 0.
Permutation PermutationRoot(const Permutation& p, unsigned root_power) {
  if (p.Size() <= 1) return p;
  std::vector<std::vector<Permutation::TCycle>> cycles_by_length(p.Size() + 1);
  for (const auto& cycle : p.Cycles()) {
    cycles_by_length[cycle.size()].push_back(cycle);
  }
  Permutation::TCycles final_cycles;
  for (unsigned i = 2; i < cycles_by_length.size(); ++i) {
    if (cycles_by_length[i].size() == 0) continue;
    unsigned pa = root_power, k = GCD(i, pa);
    for (; k != 1; k = GCD(i, pa)) pa /= k;
    unsigned l = root_power / pa;
    if ((cycles_by_length[i].size() % l) != 0) return Permutation();
    unsigned pow = TModularArithmetic_C32U::Inverse(pa, i);
    for (auto& cycle : cycles_by_length[i])
      PermutationSimpleCyclePow(cycle, pow);
    for (unsigned j = 0; j < cycles_by_length[i].size(); j += l) {
      Permutation::TCycle cycle;
      cycle.resize(i * l);
      for (unsigned k = 0; k < i * l; ++k)
        cycle[k] = cycles_by_length[i][j + (k % l)][k / l];
      final_cycles.push_back(cycle);
    }
  }
  return Permutation(p.Size(), final_cycles);
}
