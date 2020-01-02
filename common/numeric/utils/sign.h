#pragma once

template <typename T>
int Sign(const T& val) {
  return (T() < val) - (val < T());
}
