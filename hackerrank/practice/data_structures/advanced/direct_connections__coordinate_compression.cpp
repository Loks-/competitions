// https://www.hackerrank.com/challenges/direct-connections

#include "common/data_structures/binary_indexed_tree/bit.h"
#include "common/data_structures/coordinate_compression.h"
#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/stl/pair.h"
#include "common/vector/read.h"

using TPair = pair<unsigned, uint64_t>;

int main_direct_connections__coordinate_compression() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    vector<uint64_t> vx = nvector::Read<uint64_t>(N);
    vector<uint64_t> vp = nvector::Read<uint64_t>(N);
    vector<pair<uint64_t, uint64_t>> vpx;
    for (unsigned i = 0; i < N; ++i) vpx.push_back({vp[i], vx[i]});
    sort(vpx.begin(), vpx.end());
    ds::CoordinateCompression<uint64_t> cc(vx);
    ds::BIT<TPair> bit(cc.Size());
    TPair sum;
    TModularD total = 0;
    for (auto px : vpx) {
      unsigned ccx = cc.GetNew(px.second);
      TPair l = bit.Sum(ccx), r = sum - l;
      TModularD s =
          (l.first * px.second - l.second) + (r.second - r.first * px.second);
      total += s * TModularD(px.first);
      sum += make_pair(1u, px.second);
      bit.Add(ccx, {1u, px.second});
    }
    cout << total << endl;
  }
  return 0;
}
