// https://www.hackerrank.com/challenges/contacts

#include "common/hash/combine.h"
#include "common/stl/base.h"

#include <string>
#include <unordered_map>

int main_contacts() {
  unsigned N;
  cin >> N;
  unordered_map<size_t, unsigned> m;
  for (unsigned i = 0; i < N; ++i) {
    string s1, s2;
    cin >> s1 >> s2;
    size_t hvalue = 0;
    if (s1 == "add") {
      for (char c : s2) {
        nhash::DCombineH(hvalue, c);
        m[hvalue] += 1;
      }
    } else if (s1 == "find") {
      for (char c : s2) nhash::DCombineH(hvalue, c);
      cout << m[hvalue] << endl;
    }
  }
  return 0;
}
