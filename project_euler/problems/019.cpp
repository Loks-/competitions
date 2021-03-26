#include "common/date.h"
#include "common/stl/base.h"

// Time: O(T * (Y2 - Y1))
// Memory: O(1)
int main_019() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned d1, d2, m1, m2, y1, y2, n = 0;
    uint64_t yr1, yr2;
    cin >> yr1 >> m1 >> d1 >> yr2 >> m2 >> d2;
    uint64_t adjust = (yr1 / Date::YearsInCycle() - 1) * Date::YearsInCycle();
    y1 = yr1 - adjust;
    y2 = yr2 - adjust;
    Date b(d1, m1, y1), e(d2, m2, y2);
    if (d1 != 1) b = b.NextMonth();
    for (; b <= e; b = b.NextMonth())
      n += (b.DayOfWeek() == Date::SUNDAY) ? 1 : 0;
    cout << n << endl;
  }
  return 0;
}
