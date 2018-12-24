#include "common/stl/base.h"
#include "common/vector/read.h"

#include <algorithm>
#include <string>

int main_a_whole_new_word() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned N, L;
    cin >> N >> L;
    vector<string> words = ReadVector<string>(N);
    vector<unsigned> blocked_words, blocked_words_next;
    for (unsigned i = 0; i < N; ++i) blocked_words.push_back(i);
    string sfinal;
    vector<bool> vb('Z' + 1);
    vector<unsigned> vc('Z' + 1);
    for (unsigned l = 0; l < L; ++l) {
      fill(vb.begin() + 'A', vb.end(), false);
      for (unsigned i = 0; i < N; ++i) vb[words[i][l]] = true;
      fill(vc.begin() + 'A', vc.end(), 0);
      for (unsigned i : blocked_words) vc[words[i][l]] += 1;
      unsigned best = N + 1;
      char bestchar = 'A';
      for (char c = 'A'; c <= 'Z'; ++c) {
        if (!vb[c]) continue;
        if (vc[c] < best) {
          best = vc[c];
          bestchar = c;
        }
      }
      sfinal += bestchar;
      blocked_words_next.clear();
      for (unsigned i : blocked_words) {
        if (words[i][l] == bestchar) blocked_words_next.push_back(i);
      }
      blocked_words.swap(blocked_words_next);
    }
    cout << "Case #" << it << ": " << (blocked_words.size() ? "-" : sfinal)
         << endl;
  }
  return 0;
}
