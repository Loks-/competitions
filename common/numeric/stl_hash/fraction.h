#pragma once

#include "common/hash.h"
#include "common/numeric/fraction.h"

namespace std {
template <class T>
struct hash<numeric::Fraction<T>> {
  constexpr size_t operator()(const numeric::Fraction<T>& value) const {
    return HashCombine(hash<T>{}(value.GetN()), hash<T>{}(value.GetD()));
  }
};
}  // namespace std
