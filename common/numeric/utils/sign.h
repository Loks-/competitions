#pragma once

template <class T>
constexpr int Sign(const T& val) {
  return (T() < val) - (val < T());
}
