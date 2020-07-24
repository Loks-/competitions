#pragma once

#include "common/base.h"

// We assume that max size for glyph is 8*8.
class GlyphCompact {
 public:
  uint64_t mask;

  GlyphCompact();
  GlyphCompact(uint64_t _mask);
};
