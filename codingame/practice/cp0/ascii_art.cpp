// https://www.codingame.com/training/easy/ascii-art

#include "common/stl/base.h"

#include <string>

int main_ascii_art() {
  unsigned L, H;
  string m, r;
  cin >> L >> H;
  cin.ignore();
  getline(cin, m);
  for (int i = 0; i < H; i++) {
    getline(cin, r);
    for (char c : m) {
      unsigned index = isalpha(c) ? toupper(c) - 'A' : 26;
      cout << r.substr(L * index, L);
    }
    cout << endl;
  }
  return 0;
}
