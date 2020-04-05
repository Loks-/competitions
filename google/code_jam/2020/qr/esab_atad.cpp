#include "common/stl/base.h"

#include <string>

int main_esab_atad() {
  unsigned T, B;
  cin >> T >> B;
  auto I2B = [&](unsigned i) { return (i & 1) ? (B - i / 2) : (i / 2 + 1); };
  auto B2I = [&](unsigned b) {
    return (b <= B / 2) ? 2 * (b - 1) : (2 * (B - b) + 1);
  };

  for (unsigned it = 0; it < T; ++it) {
    vector<unsigned> v(B);
    unsigned same_value = B, opp_value = B;
    unsigned step = 0, index = 0, t;
    for (; index < B; step += 2) {
      if (step && ((step % 10) == 0)) {
        if (same_value < B) {
          cout << I2B(same_value) << endl;
          cin >> t;
          if (v[same_value] != t) {
            for (unsigned i = 0; i < index; ++i) {
              v[i] = 1 - v[i];
            }
          }
        } else {
          cout << B << endl;
          cin >> t;
        }
        if (opp_value < B) {
          cout << I2B(opp_value) << endl;
          cin >> t;
          if (v[opp_value] != t) {
            for (unsigned i = 0; i < index; i += 2) {
              swap(v[i], v[i + 1]);
            }
          }
        } else {
          cout << B << endl;
          cin >> t;
        }
      } else {
        cout << I2B(index) << endl << I2B(index + 1) << endl;
        cin >> v[index] >> v[index + 1];
        if (v[index] == v[index + 1]) {
          same_value = index;
        } else {
          opp_value = index;
        }
        index += 2;
      }
    }
    for (unsigned b = 1; b <= B; ++b) cout << v[B2I(b)];
    cout << endl;
    char c;
    cin >> c;
    if (c != 'Y') break;
  }
  return 0;
}
