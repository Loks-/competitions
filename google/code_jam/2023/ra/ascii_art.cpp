#include "common/algorithm/binary_search.h"
#include "common/stl/base.h"

int main_ascii_art() {
  unsigned T;
  uint64_t N, M = 26;

  auto F = [](uint64_t k) { return (k * (k + 1)) / 2; };

  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    N -= 1;
    auto k = UpperBoundF(0u, (1u << 20), N / M, F);
    cout << "Case #" << it << ": " << char('A' + ((N - M * F(k - 1)) / k))
         << endl;
  }
  return 0;
}
