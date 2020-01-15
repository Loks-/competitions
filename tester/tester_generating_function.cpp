#include "common/generating_function/functions/geometric.h"
#include "common/generating_function/functions/partition.h"
#include "common/generating_function/operators/partition.h"
#include "common/modular.h"

bool TestGenetatingFunctionOperatorsPartition() {
  uint64_t n = 100, expected = 190569292;
  auto f_uint64_t = gf::operators::MakePartition(
      gf::functions::MakePGeometricOne<uint64_t>());
  auto f_modular = gf::operators::MakePartition(
      gf::functions::MakePGeometricOne<TModularD>());
  return (f_uint64_t->Get(n) == expected) &&
         (f_modular->Get(n) == TModularD(expected));
}

bool TestGenetatingFunctionFunctionsPartition() {
  uint64_t n = 100, expected = 190569292;
  auto f_uint64_t = gf::functions::MakePartition<uint64_t>();
  auto f_modular = gf::functions::MakePartition<TModularD>();
  return (f_uint64_t->Get(n) == expected) &&
         (f_modular->Get(n) == TModularD(expected));
}

bool TestGeneratingFunction() {
  bool all_ok = true;
  all_ok = all_ok && TestGenetatingFunctionOperatorsPartition();
  all_ok = all_ok && TestGenetatingFunctionFunctionsPartition();
  return all_ok;
}
