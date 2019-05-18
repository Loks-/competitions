#include "common/stl/base.h"

int main_pottery_lottery() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    int day;
    for (unsigned j = 0; j < 100; ++j) {
      cin >> day;
      if (j < 19) {
        cout << j + 1 << " " << 100 << endl;
      } else if (j < 99) {
        cout << (j % 19) + 1 << " " << j + 1 << endl;
      } else {
        cout << 20 << " " << 100 << endl;
      }
    }
  }
  return 0;
}
