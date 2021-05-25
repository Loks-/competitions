// https://www.codingame.com/training/easy/container-terminal

#include "common/stl/base.h"
#include "common/vector/longest_Increasing_subsequence.h"

#include <string>

int main_container_terminal() {
  unsigned N;
  cin >> N;
  for (unsigned i = 0; i < N; ++i) {
    string s;
    cin >> s;
    std::vector<char> v(s.begin(), s.end());
    cout << nvector::LongestIncresingSubsequence(v) << endl;
  }
  return 0;
}
