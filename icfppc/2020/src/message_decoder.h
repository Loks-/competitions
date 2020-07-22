#pragma once

#include "expression.h"
#include "glyph.h"
#include "glyph_compact.h"
#include "line.h"
#include "matrix_slice.h"
#include "message.h"
#include "message_as_image.h"
#include "message_as_text.h"

#include "common/linear_algebra/matrix.h"

#include <string>

class GlyphDecoder;

class MessageDecoder {
 protected:
  GlyphDecoder& gd;

 public:
  using TMatrixSlice = MatrixSlice<la::MatrixBool>;

  MessageDecoder();
  MessageDecoder(GlyphDecoder& _gd);

  const TMatrixSlice Compress(const TMatrixSlice& ms);
  GlyphCompact DecodeGlyphCompact(const TMatrixSlice& ms);
  Glyph DecodeLEFGlyph(const TMatrixSlice& ms);
  Glyph DecodeGlyph(const TMatrixSlice& ms);
  Expression DecodeExpression(const TMatrixSlice& ms);
  Line DecodeLine(const TMatrixSlice& s);
  Message Decode(const MessageAsImage& mi);

  Glyph DecodeGlyph(const std::string& s);
  Expression DecodeExpression(const std::string& s);
  Line DecodeLine(const std::string& s);
  Message Decode(const MessageAsText& mt);
};
