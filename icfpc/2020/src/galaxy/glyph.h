#pragma once

#include "galaxy/function_type.h"
#include "galaxy/glyph_type.h"
#include "galaxy/linear_encoded_form.h"
#include "galaxy/picture.h"

#include "common/base.h"

class Glyph {
 public:
  GlyphType type;
  FunctionType ftype;
  int64_t value;
  LEF lef;
  Picture pic;

  Glyph();
  Glyph(GlyphType _type);
  Glyph(FunctionType _ftype);
  Glyph(GlyphType _type, int64_t _value);
  Glyph(const LEF& _lef);
  Glyph(const Picture& _pic);

  bool operator==(const Glyph& g) const;

  void Print() const;
};
