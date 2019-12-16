#include "common/generating_function/functions/geometric.h"
#include "common/generating_function/functions/partition.h"
#include "common/generating_function/operators/partition.h"
#include "common/modular.h"

bool TestGenetatingFunctionOperatorsPartition() {
  uint64_t n = 100, expected = 190569292;
  auto f_uint64_t = gf::operators::CreatePartition(
      gf::functions::CreatePGeometricOne<uint64_t>());
  auto f_modular = gf::operators::CreatePartition(
      gf::functions::CreatePGeometricOne<TModularD>());
  return (f_uint64_t->Get(n) == expected) &&
         (f_modular->Get(n) == TModularD(expected));
}

bool TestGenetatingFunctionFunctionsPartition() {
  uint64_t n = 100, expected = 190569292;
  auto f_uint64_t = gf::functions::CreatePartition<uint64_t>();
  auto f_modular = gf::functions::CreatePartition<TModularD>();
  return (f_uint64_t->Get(n) == expected) &&
         (f_modular->Get(n) == TModularD(expected));
}

bool TestGeneratingFunction() {
  bool all_ok = true;
  all_ok = all_ok && TestGenetatingFunctionOperatorsPartition();
  all_ok = all_ok && TestGenetatingFunctionFunctionsPartition();
  return all_ok;
}
