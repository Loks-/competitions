// https://www.hackerrank.com/challenges/direct-connections

#include "common/binary_indexed_tree/bit.h"
#include "common/coordinate_compression.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"
#include "common/stl/pair.h"
#include "common/vector/read.h"

using TModular = Modular<>;
using TPair = pair<unsigned, uint64_t>;

int main_direct_connections__coordinate_compression() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    vector<uint64_t> vx = ReadVector<uint64_t>(N);
    vector<uint64_t> vp = ReadVector<uint64_t>(N);
    vector<pair<uint64_t, uint64_t>> vpx;
    for (unsigned i = 0; i < N; ++i) vpx.push_back({vp[i], vx[i]});
    sort(vpx.begin(), vpx.end());
    CoordinateCompression<uint64_t> cc;
    cc.InitUnsorted(vx);
    BIT<TPair> bit(cc.Size());
    TPair sum;
    TModular total = 0;
    for (auto px : vpx) {
      unsigned ccx = cc.GetNew(px.second);
      TPair l = bit.Sum(ccx), r = sum - l;
      TModular s =
          (l.first * px.second - l.second) + (r.second - r.first * px.second);
      total += s * TModular(px.first);
      sum += make_pair(1u, px.second);
      bit.Add(ccx, {1u, px.second});
    }
    cout << total << endl;
  }
  return 0;
}
