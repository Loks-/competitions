#pragma once

#include <array>
#include <utility>

namespace hidden {
template <class TValue, std::size_t... Is>
constexpr std::array<TValue, sizeof...(Is)> MakeArray(
    const TValue& value, std::index_sequence<Is...>) {
  return {{(static_cast<void>(Is), value)...}};
}
}

template <class TValue, size_t N>
constexpr std::array<TValue, N> MakeArray(const TValue& value) {
  return hidden::MakeArray(value, std::make_index_sequence<N>());
}
