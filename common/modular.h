#include "common/modular/static/modular.h"

template <uint64_t mod>
using TModular_P32 = modular::mstatic::Modular<mod, true, true>;

template <uint64_t mod>
using TModular_C32 = modular::mstatic::Modular<mod, false, true>;

using TModularD = TModular_P32<1000000007>;
