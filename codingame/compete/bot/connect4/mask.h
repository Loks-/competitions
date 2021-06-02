#pragma once

#include "board.h"

using TMask = uint64_t;

const TMask mask_1 = 1, mask_all = (mask_1 << SIZE) - 1,
            mask_c0 = (mask_1 << HEIGHT) - 1, mask_r0 = mask_all / mask_c0;

inline bool Has4(TMask mask) {
  static const TMask mh = (mask_all >> (3 * HEIGHT)),
                     mv = mask_r0 * ((mask_1 << (HEIGHT - 3)) - 1),
                     md1 = mh & mv, md2 = (md1 << (3 * HEIGHT));
  static const unsigned D1 = HEIGHT + 1, D2 = HEIGHT - 1, H = HEIGHT, V = 1;
  TMask d1 = mask & (mask >> D1), d2 = mask & (mask >> D2),
        h = mask & (mask >> H), v = mask & (mask >> V);
  return (md1 & d1 & (d1 >> (2 * D1))) | (md2 & d2 & (d2 >> (2 * D2))) |
         (mh & h & (h >> (2 * H))) | (mv & v & (v >> (2 * V)));
}
