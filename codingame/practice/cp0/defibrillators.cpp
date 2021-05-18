// https://www.codingame.com/training/easy/defibrillators

#include "common/stl/base.h"

#include <cmath>
#include <string>

int main_defibrillators() {
  auto Read = [](char del) {
    string s;
    getline(cin, s, del);
    replace(s.begin(), s.end(), ',', '.');
    return stod(s) * 4 * atan(1.0) / 180;
  };

  double lon = Read('\n'), lat = Read('\n'), best_value = 4.;
  unsigned N;
  string best_name, current, unused;
  cin >> N;
  for (unsigned i = 0; i < N; ++i) {
    getline(cin, unused, ';');
    getline(cin, current, ';');
    getline(cin, unused, ';');
    getline(cin, unused, ';');
    double clon = Read(';'), clat = Read('\n'),
           dx = (lon - clon) * cos((lat + clat) / 2), dy = lat - clat,
           d = dx * dx + dy * dy;
    if (best_value > d) {
      best_value = d;
      best_name = current;
    }
  }
  cout << best_name << endl;
  return 0;
}
