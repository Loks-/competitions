// https://www.codingame.com/training/easy/chuck-norris

#include "common/stl/base.h"

#include <string>

int main_chuck_norris() {
  string s;
  getline(cin, s);
  char m = 64, p = s[0] & m;
  cout << (p ? "0 " : "00 ");
  for (char c : s) {
    for (unsigned i = 0; i < 7; ++i) {
      char b = c & m;
      cout << ((b == p) ? "0" : (p = b) ? " 0 0" : " 00 0");
      c <<= 1;
    }
  }
  cout << endl;
  return 0;
}
