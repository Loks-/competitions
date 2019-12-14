#include "common/generating_function/functions/partition.h"
#include "common/modular.h"

bool TestGenetatingFunctionPartition() {
  uint64_t n = 100, expected = 190569292;
  auto f_uint64_t = gf::functions::CreatePartition<uint64_t>();
  auto f_modular = gf::functions::CreatePartition<TModularD>();
  return (f_uint64_t->Get(n) == expected) &&
         (f_modular->Get(n) == TModularD(expected));
}

bool TestGeneratingFunction() { return TestGenetatingFunctionPartition(); }
