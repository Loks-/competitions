#include "galaxy/glyph_decoder.h"

#include "common/assert_exception.h"
#include "common/base.h"

#include <vector>

namespace galaxy {
namespace {
uint64_t MASK_DOT = 1 << 8;

GlyphDecoder gd;
}  // namespace

void GlyphDecoder::RegisterGlyphType(GlyphType type, const std::string& id,
                                     uint64_t mask) {
  Assert((mask & 1) || (mask == MASK_DOT));
  Assert(map_mask_glyph_type.find(mask) == map_mask_glyph_type.end());
  Assert(map_glyph_type_mask.find(type) == map_glyph_type_mask.end());
  Assert(map_id_glyph_type.find(id) == map_id_glyph_type.end());
  Assert(map_glyph_type_id.find(type) == map_glyph_type_id.end());
  map_mask_glyph_type[mask] = type;
  map_glyph_type_mask[type] = mask;
  map_id_glyph_type[id] = type;
  map_glyph_type_id[type] = id;
}

void GlyphDecoder::RegisterFunctionType(FunctionType type,
                                        const std::string& id, uint64_t mask) {
  Assert(map_mask_function_type.find(mask) == map_mask_function_type.end());
  Assert(map_function_type_mask.find(type) == map_function_type_mask.end());
  Assert(map_id_function_type.find(id) == map_id_function_type.end());
  Assert(map_function_type_id.find(type) == map_function_type_id.end());
  if (mask) {
    map_mask_function_type[mask] = type;
    map_function_type_mask[type] = mask;
  }
  map_id_function_type[id] = type;
  map_function_type_id[type] = id;
}

GlyphDecoder::GlyphDecoder() { InitMap(); }

void GlyphDecoder::InitMap() {
  rows.resize(8, 1);
  for (unsigned i = 1; i < 8; ++i) rows[i] = (rows[i - 1] << 8);
  RegisterGlyphType(GlyphType::ONE, "X", rows[0] + rows[1]);
  RegisterGlyphType(GlyphType::DOT, "...", MASK_DOT);
  RegisterGlyphType(GlyphType::EQUALITY, "=",
                    7 * rows[0] + rows[1] + 7 * rows[2]);
  RegisterGlyphType(GlyphType::AP, "ap", 3 * rows[0] + rows[1]);
  RegisterFunctionType(FunctionType::SUCCESSOR, "inc",
                       15 * rows[0] + 3 * rows[1] + 9 * rows[2] + 13 * rows[3]);
  RegisterFunctionType(FunctionType::PREDECESSOR, "dec",
                       15 * rows[0] + 3 * rows[1] + 5 * rows[2] + 13 * rows[3]);
  RegisterFunctionType(
      FunctionType::SUM, "add",
      15 * rows[0] + 11 * rows[1] + 11 * rows[2] + 11 * rows[3]);
  RegisterFunctionType(FunctionType::PRODUCT, "mul",
                       15 * rows[0] + 5 * rows[1] + 5 * rows[2] + 5 * rows[3]);
  RegisterFunctionType(FunctionType::DIVISION, "div",
                       15 * rows[0] + rows[1] + 11 * rows[2] + rows[3]);
  RegisterFunctionType(FunctionType::EQUALITY, "eq",
                       15 * rows[0] + rows[1] + rows[2] + 15 * rows[3]);
  RegisterFunctionType(FunctionType::STRICT_LESS, "lt",
                       15 * rows[0] + rows[1] + 9 * rows[2] + 13 * rows[3]);
  RegisterFunctionType(FunctionType::MODULATE, "mod",
                       15 * rows[0] + 5 * rows[1] + 11 * rows[2] + 5 * rows[3]);
  RegisterFunctionType(
      FunctionType::DEMODULATE, "dem",
      15 * rows[0] + 11 * rows[1] + 5 * rows[2] + 11 * rows[3]);
  RegisterFunctionType(
      FunctionType::SEND, "send",
      15 * rows[0] + 13 * rows[1] + 11 * rows[2] + 9 * rows[3]);
  RegisterFunctionType(FunctionType::NEGATE, "neg",
                       7 * rows[0] + 5 * rows[1] + 5 * rows[2]);
  RegisterFunctionType(FunctionType::S_COMBINATOR, "s",
                       7 * rows[0] + 7 * rows[1] + 3 * rows[2]);
  RegisterFunctionType(FunctionType::C_COMBINATOR, "c",
                       7 * rows[0] + 5 * rows[1] + 3 * rows[2]);
  RegisterFunctionType(FunctionType::B_COMBINATOR, "b",
                       7 * rows[0] + 3 * rows[1] + 3 * rows[2]);
  RegisterFunctionType(FunctionType::K_COMBINATOR, "t",
                       7 * rows[0] + 5 * rows[1] + 1 * rows[2]);
  RegisterFunctionType(FunctionType::FALSE__SECOND, "f",
                       7 * rows[0] + 1 * rows[1] + 5 * rows[2]);
  RegisterFunctionType(FunctionType::POWER_OF_TWO, "pwr2",
                       127 * rows[0] + 65 * rows[1] + 89 * rows[2] +
                           85 * rows[3] + 69 * rows[4] + 65 * rows[5] +
                           127 * rows[6]);
  RegisterFunctionType(FunctionType::I_COMBINATOR, "i",
                       3 * rows[0] + 3 * rows[1]);
  RegisterFunctionType(
      FunctionType::CONS__PAIR, "cons",
      31 * rows[0] + 21 * rows[1] + 21 * rows[2] + 21 * rows[3] + 31 * rows[4]);
  RegisterFunctionType(
      FunctionType::CAR__FIRST, "car",
      31 * rows[0] + 29 * rows[1] + 21 * rows[2] + 21 * rows[3] + 31 * rows[4]);
  RegisterFunctionType(
      FunctionType::CDR__TAIL, "cdr",
      31 * rows[0] + 23 * rows[1] + 21 * rows[2] + 21 * rows[3] + 31 * rows[4]);
  RegisterFunctionType(FunctionType::NIL__EMPTY_LIST, "nil",
                       7 * rows[0] + 5 * rows[1] + 7 * rows[2]);
  RegisterFunctionType(FunctionType::IS_NIL, "isnil",
                       7 * rows[0] + 7 * rows[1] + 7 * rows[2]);
  RegisterFunctionType(FunctionType::VECTOR, "vec",
                       63 * rows[0] + 3 * rows[1] + 5 * rows[2] + 9 * rows[3] +
                           17 * rows[4] + 33 * rows[5]);
  RegisterFunctionType(FunctionType::DRAW, "draw",
                       63 * rows[0] + 33 * rows[1] + 33 * rows[2] +
                           33 * rows[3] + 33 * rows[4] + 63 * rows[5]);
  RegisterFunctionType(FunctionType::MULTIPLE_DRAW, "multipledraw",
                       127 * rows[0] + 73 * rows[1] + 73 * rows[2] +
                           127 * rows[3] + 73 * rows[4] + 73 * rows[5] +
                           127 * rows[6]);
  RegisterFunctionType(
      FunctionType::IF0, "if0",
      31 * rows[0] + 1 * rows[1] + 29 * rows[2] + 7 * rows[3] + 29 * rows[4]);
  RegisterFunctionType(FunctionType::MODEM, "modem",
                       63 * rows[0] + 1 * rows[1] + 1 * rows[2] + 1 * rows[3] +
                           51 * rows[4] + 13 * rows[5]);
  RegisterFunctionType(FunctionType::F38, "f38",
                       63 * rows[0] + 37 * rows[1] + 61 * rows[2] +
                           47 * rows[3] + 41 * rows[4] + 63 * rows[5]);
  RegisterFunctionType(FunctionType::INTERACT, "interact",
                       63 * rows[0] + 33 * rows[1] + 45 * rows[2] +
                           45 * rows[3] + 33 * rows[4] + 63 * rows[5]);
  RegisterFunctionType(FunctionType::GALAXY, "galaxy",
                       28 * rows[0] + 32 * rows[1] + 78 * rows[2] +
                           85 * rows[3] + 57 * rows[4] + 2 * rows[5] +
                           28 * rows[6]);
}

Glyph GlyphDecoder::Decode(const GlyphCompact& gc) const {
  if (gc.mask == MASK_DOT) return {GlyphType::DOT};
  if ((gc.mask & 131) == 130) return {GlyphType::NUMBER, DecodeNumber(gc.mask)};
  auto itf = map_mask_function_type.find(gc.mask);
  if (itf != map_mask_function_type.end())
    return {itf->second != FunctionType::VECTOR ? itf->second
                                                : FunctionType::CONS__PAIR};
  auto itg = map_mask_glyph_type.find(gc.mask);
  if (itg != map_mask_glyph_type.end()) return {itg->second};
  if (IsVariable(gc.mask))
    return {GlyphType::VARIABLE, DecodeVariable(gc.mask)};
  return {};
}

GlyphCompact GlyphDecoder::Encode(const Glyph& g) const {
  switch (g.type) {
    case GlyphType::NUMBER:
      return {EncodeNumber(g.value)};
    case GlyphType::FUNCTION: {
      auto itf = map_function_type_mask.find(g.ftype);
      Assert(itf != map_function_type_mask.end());
      return {itf->second};
    }
    case GlyphType::VARIABLE:
      return {EncodeVariable(g.value)};
    default: {
      auto itg = map_glyph_type_mask.find(g.type);
      Assert(itg != map_glyph_type_mask.end());
      return {itg->second};
    }
  }
}

bool GlyphDecoder::DecodeNumberSign(uint64_t mask) const {
  for (unsigned i = 1; i < 8; ++i) {
    if ((mask & (1ull << (8 * i))) && !(mask & (1ull << i))) return true;
  }
  return false;
}

int64_t GlyphDecoder::DecodeNumber(uint64_t gmask) const {
  const uint64_t mask = (1u << 7) - 1;
  int64_t m = ((gmask >> 1) & mask) + 1, p = 1, r = 0;
  for (unsigned i = 1; i < 8; ++i) {
    r += p * ((gmask >> (8 * i + 1)) & mask);
    p *= m;
  }
  return {DecodeNumberSign(gmask) ? -r : r};
}

uint64_t GlyphDecoder::EncodeNumber(int64_t value) const {
  uint64_t avalue = (value < 0 ? -value : value);
  unsigned i = 1;
  for (; i < 7; ++i) {
    if (avalue < (1ull << (i * i))) break;
  }
  unsigned mask = (1ull << i) - 1, r = mask << 1;
  for (unsigned j = 0; j < i; ++j)
    r += ((1 + 2 * (((avalue >> (i * j)) & mask))) << (8 * (j + 1)));
  if (value < 0) r += (1ull << (8 * (i + 1)));
  return r;
}

// Current version support only positive indexes for variables
bool GlyphDecoder::IsVariable(uint64_t mask) const {
  unsigned i = 4;
  for (; i < 8; ++i) {
    if ((mask & (1ull << i)) == 0) break;
  }
  uint64_t a = (1ull << i) - 1, b = a + (a << (8 * (i - 1))),
           c = 1 + (1ull << (i - 1));
  if ((mask & b) != b) return false;
  for (unsigned j = 1; j < i - 1; ++j) {
    if (((mask >> (8 * j)) & c) != c) return false;
  }
  return true;
}

int64_t GlyphDecoder::DecodeVariable(uint64_t mask) const {
  unsigned i = 4;
  for (; i < 8; ++i) {
    if ((mask & (1ull << i)) == 0) break;
  }
  for (unsigned j = 0; j < i; ++j) mask ^= ((1ull << i) - 1) << (8 * j);
  return DecodeNumber(mask >> 9);
}

uint64_t GlyphDecoder::EncodeVariable(int64_t value) const {
  assert(value >= 0);
  unsigned i = 1;
  for (; i < 7; ++i) {
    if (value < (1ll << (i * i))) break;
  }
  i += 3;
  uint64_t mask = EncodeNumber(value) << 9;
  for (unsigned j = 0; j < i; ++j) mask ^= ((1ull << i) - 1) << (8 * j);
  return mask;
}

Glyph GlyphDecoder::Decode(const std::string& name) {
  assert(name.size() > 0);
  auto itf = map_id_function_type.find(name);
  if (itf != map_id_function_type.end())
    return {itf->second != FunctionType::VECTOR ? itf->second
                                                : FunctionType::CONS__PAIR};
  auto itg = map_id_glyph_type.find(name);
  if (itg != map_id_glyph_type.end()) return {itg->second};
  if (name[0] == 'x') return {GlyphType::VARIABLE, std::stoll(name.substr(1))};
  if (name[0] == '[') {
    assert(name.back() == ']');
    return {(name == "[nil]")
                ? LEFEncodeNIL()
                : LEFEncodeNumber(std::stoll(name.substr(1, name.size() - 2)))};
  }
  if (name[0] == ':') return {GlyphType::ALIAS, std::stoll(name.substr(1))};
  if (name == ")") return {FunctionType::NIL__EMPTY_LIST};
  return {GlyphType::NUMBER, std::stoll(name)};
}

std::string GlyphDecoder::ToString(const Glyph& g) const {
  switch (g.type) {
    case GlyphType::NUMBER:
      return std::to_string(g.value);
    case GlyphType::FUNCTION: {
      auto itf = map_function_type_id.find(g.ftype);
      Assert(itf != map_function_type_id.end());
      return {itf->second};
    }
    case GlyphType::VARIABLE:
      return "x" + std::to_string(g.value);
    case GlyphType::PICTURE:
      return g.pic.ToString();
    default: {
      auto itg = map_glyph_type_id.find(g.type);
      Assert(itg != map_glyph_type_id.end());
      return {itg->second};
    }
  }
}

GlyphDecoder& GlyphDecoder::GetDecoder() { return gd; }
}  // namespace galaxy
