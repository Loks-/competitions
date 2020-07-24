#pragma once

#include "galaxy/expression.h"
#include "galaxy/glyph.h"
#include "galaxy/glyph_compact.h"
#include "galaxy/line.h"
#include "galaxy/matrix_slice.h"
#include "galaxy/message.h"
#include "galaxy/message_as_image.h"
#include "galaxy/message_as_text.h"

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
