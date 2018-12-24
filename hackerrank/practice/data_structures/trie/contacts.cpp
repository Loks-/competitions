// https://www.hackerrank.com/challenges/contacts

#include "common/hash.h"
#include "common/stl/base.h"

#include <string>
#include <unordered_map>

int main_contacts() {
  unsigned N;
  cin >> N;
  unordered_map<size_t, unsigned> m1, m2;
  hash<char> h;
  for (unsigned i = 0; i < N; ++i) {
    string s1, s2;
    cin >> s1 >> s2;
    size_t hvalue1 = 0, hvalue2 = 1;
    if (s1 == "add") {
      for (char c : s2) {
        hvalue1 = hash_combine(hvalue1, h(c));
        hvalue2 = hash_combine(hvalue2, h(c));
        m1[hvalue1] += 1;
        m2[hvalue2] += 1;
      }
    } else if (s1 == "find") {
      for (char c : s2) {
        hvalue1 = hash_combine(hvalue1, h(c));
        hvalue2 = hash_combine(hvalue2, h(c));
      }
      cout << min(m1[hvalue1], m2[hvalue2]) << endl;
    }
  }
  return 0;
}
