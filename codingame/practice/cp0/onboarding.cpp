// https://www.codingame.com/training/easy/onboarding

#include "common/stl/base.h"

#include <string>

int main_onboarding() {
  for (;;) {
    string e1, e2;
    int d1, d2;
    cin >> e1 >> d1 >> e2 >> d2;
    cout << ((d1 < d2) ? e1 : e2) << endl;
  }
  return 0;
}
