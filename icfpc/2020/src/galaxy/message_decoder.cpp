#include "galaxy/message_decoder.h"

#include "galaxy/glyph_decoder.h"

#include "common/base.h"
#include "common/linear_algebra/bool/vector.h"
#include "common/string/utils/split.h"

namespace galaxy {
MessageDecoder::MessageDecoder() : gd(GlyphDecoder::GetDecoder()) {}

MessageDecoder::MessageDecoder(GlyphDecoder& _gd) : gd(_gd) {}

const MessageDecoder::TMatrixSlice MessageDecoder::Compress(
    const TMatrixSlice& ms) {
  unsigned re = ms.Rows() - 1;
  for (; (re > 0) && ms.IsRowEmpty(re);) --re;
  return TMatrixSlice(ms, 0, re + 1);
}

GlyphCompact MessageDecoder::DecodeGlyphCompact(const TMatrixSlice& ms) {
  assert((ms.Rows() <= 8) && (ms.Columns() <= 8));
  uint64_t v = 0;
  for (unsigned r = 0; r < ms.Rows(); ++r) {
    for (unsigned c = 0; c < ms.Columns(); ++c) {
      v += (ms.Get(r, c) ? 1ull : 0ull) << (8 * r + c);
    }
  }
  return {v};
}

Glyph MessageDecoder::DecodeLEFGlyph(const TMatrixSlice& ms) {
  assert(ms.Rows() == 2);
  LEF v(ms.Columns());
  for (unsigned c = 0; c < ms.Columns(); ++c) {
    assert(ms.Get(0, c) != ms.Get(1, c));
    v[c] = ms.Get(0, c);
  }
  return {v};
}

Glyph MessageDecoder::DecodeGlyph(const TMatrixSlice& ms) {
  return ((ms.Rows() == 2) && (ms.Columns() > 2))
             ? DecodeLEFGlyph(ms)
             : gd.Decode(DecodeGlyphCompact(ms));
}

Expression MessageDecoder::DecodeExpression(const TMatrixSlice& ms) {
  Expression e;
  for (unsigned cb = 0, ce; cb < ms.Columns();) {
    assert(!ms.IsColumnEmpty(cb));
    for (ce = cb; ce < ms.Columns(); ++ce) {
      if (ms.IsColumnEmpty(ce)) break;
    }
    e.v.push_back(
        DecodeGlyph(Compress(TMatrixSlice(ms, 0, ms.Rows(), cb, ce))));
    cb = ce + 1;
    if (ms.IsColumnEmpty(cb)) ++cb;
  }
  return e;
}

Line MessageDecoder::DecodeLine(const TMatrixSlice& ms) {
  Line l;
  for (unsigned cb = 0, ce; cb < ms.Columns();) {
    assert(!ms.IsColumnEmpty(cb));
    for (ce = cb; ce + 2 < ms.Columns(); ++ce) {
      if (ms.IsColumnEmpty(ce) && ms.IsColumnEmpty(ce + 1) &&
          ms.IsColumnEmpty(ce + 2))
        break;
    }
    l.v.push_back(DecodeExpression(TMatrixSlice(ms, 0, ms.Rows(), cb, ce + 2)));
    for (cb = ce + 3; (cb < ms.Columns()) && ms.IsColumnEmpty(cb);) ++cb;
  }
  return l;
}

Message MessageDecoder::Decode(const MessageAsImage& mi) {
  Message m;
  TMatrixSlice ms(mi.m);
  assert(!ms.IsRowEmpty(0));
  for (unsigned rb = 0, re; rb < ms.Rows();) {
    for (re = rb; re + 1 < ms.Rows(); ++re) {
      if (ms.IsRowEmpty(re) && ms.IsRowEmpty(re + 1)) break;
    }
    m.v.push_back(DecodeLine(TMatrixSlice(ms, rb, re)));
    if ((re == rb + 2) && ms.IsRowEmpty(rb)) ++re;
    rb = re + 2;
  }
  return m;
}

Glyph MessageDecoder::DecodeGlyph(const std::string& s) { return gd.Decode(s); }

Expression MessageDecoder::DecodeExpression(const std::string& s) {
  if (s[0] == '[') {
    assert(s.back() == ']');
    auto e = DecodeExpression(s.substr(1, s.size() - 2));
    LEF lef = LEFEncodeNode(e.MakeRoot());
    return Expression(Glyph(lef));
  }
  Expression e;
  auto vs = Split(s, ' ');
  for (size_t i = 0; i < vs.size(); ++i) {
    auto& g = vs[i];
    if (g == "(") {
      assert(i + 1 < vs.size());
      if (vs[i + 1] != ")") {
        e.v.push_back(Glyph(GlyphType::AP));
        e.v.push_back(Glyph(GlyphType::AP));
        e.v.push_back(Glyph(FunctionType::CONS__PAIR));
      }
    } else if (g == ",") {
      e.v.push_back(Glyph(GlyphType::AP));
      e.v.push_back(Glyph(GlyphType::AP));
      e.v.push_back(Glyph(FunctionType::CONS__PAIR));
    } else {
      e.v.push_back(DecodeGlyph(g));
    }
  }
  return e;
}

Line MessageDecoder::DecodeLine(const std::string& s) {
  Line l;
  size_t npos0 = 0, npos;
  for (npos = s.find("  ", npos0); npos != std::string::npos;
       npos = s.find("  ", npos0)) {
    l.v.push_back(DecodeExpression(s.substr(npos0, npos - npos0)));
    npos0 = npos;
    for (; (npos0 < s.size()) && (s[npos0] == ' ');) ++npos0;
  }
  if (npos0 < s.size()) l.v.push_back(DecodeExpression(s.substr(npos0)));
  return l;
}

Message MessageDecoder::Decode(const MessageAsText& mt) {
  Message m;
  for (auto& l : mt.m) m.v.push_back(DecodeLine(l));
  return m;
}
}  // namespace galaxy
