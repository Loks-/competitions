#include "common/stl/base.h"

#include <iomanip>

int main_765() {
  double h = .5, p = .6, q = 1 - p;
  std::vector<std::pair<double, double>> current, next;
  current.push_back({0, 0});
  current.push_back({1e12, 1});
  for (unsigned r = 0; r < 1000; ++r) {
    next.resize(current.size() + 1);
    next[0] = current[0];
    for (unsigned i = 1; i < current.size(); ++i)
      next[i] = {h * current[i - 1].first + h * current[i].first,
                 q * current[i - 1].second + p * current[i].second};
    next.back() = current.back();
    current.swap(next);
  }
  auto it = std::lower_bound(current.begin(), current.end(),
                             std::make_pair(1.0, 0.0));
  auto x1 = *(it - 1), x2 = *it;
  auto fp = ((x2.first - 1) * x1.second + (1 - x1.first) * x2.second) /
            (x2.first - x1.first);
  std::cout << setprecision(10) << fixed << fp << std::endl;
  return 0;
}
