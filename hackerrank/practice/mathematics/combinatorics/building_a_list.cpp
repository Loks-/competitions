// https://www.hackerrank.com/challenges/building-a-list

#include "common/stl/base.h"

#include <string>

int main_building_a_list() {
  unsigned T, N;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> N;
    string s, st;
    cin >> s;
    sort(s.begin(), s.end());
    vector<string> vs;
    for (unsigned i = 1; i < (1u << N); ++i) {
      st = "";
      for (unsigned j = 0; j < N; ++j) {
        if (i & (1u << j)) st += s[j];
      }
      vs.push_back(st);
    }
    sort(vs.begin(), vs.end());
    for (const string& sf : vs) cout << sf << endl;
  }
  return 0;
}
