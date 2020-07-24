#include "galaxy/linear_encoded_form.h"

#include "galaxy/expression.h"
#include "galaxy/node.h"

#include "common/base.h"

LEF LEFEncodeNumber(int64_t value) {
  int64_t avalue = (value < 0) ? -value : value;
  unsigned l = 0;
  for (; (1ll << (4 * l)) <= avalue;) ++l;
  LEF v(2 + (l + 1) + 4 * l);
  v[(value < 0) ? 0 : 1] = 1;
  for (unsigned i = 0; i < l; ++i) v[i + 2] = 1;
  unsigned k = l + 2;
  for (unsigned i = 0; i < 4 * l; ++i) {
    if (avalue & (1ll << i)) v[k + 4 * l - i] = 1;
  }
  return v;
}

LEF LEFEncodeNIL() { return LEF(2, false); }

LEF LEFEncodeCons() { return LEF(2, true); }

LEF LEFEncodePair(Node* node) {
  LEF v = LEFEncodeCons(), v1 = LEFEncodeNode(node->l->r),
      v2 = LEFEncodeNode(node->r);
  v.insert(v.end(), v1.begin(), v1.end());
  v.insert(v.end(), v2.begin(), v2.end());
  return v;
}

LEF LEFEncodeNode(Node* node) {
  assert(node);
  if (node->data.type == GlyphType::NUMBER)
    return LEFEncodeNumber(node->data.value);
  if (IsNil(node)) return LEFEncodeNIL();
  if (IsPair(node)) return LEFEncodePair(node);
  assert(false);
  return {};
}

int64_t LEFDecodeNumber(const LEF& lef, unsigned& index) {
  assert(lef[index] != lef[index + 1]);
  int64_t sign = lef[index] ? -1 : 1;
  unsigned k = 2;
  for (; lef[index + k];) ++k;
  unsigned l = k - 2;
  assert(lef.size() >= index + 5 * l + 3);
  int64_t value = 0;
  for (unsigned i = 0; i < 4 * l; ++i) {
    if (lef[index + k + 4 * l - i]) value += (1ll << i);
  }
  index += 5 * l + 3;
  return sign * value;
}

int64_t LEFDecodeNumber(const LEF& lef) {
  unsigned index = 0;
  auto r = LEFDecodeNumber(lef, index);
  assert(index == lef.size());
  return r;
}

Node* LEFDecodeExpression(const LEF& lef) {
  Expression e;
  unsigned index = 0;
  for (; index < lef.size();) {
    if (lef[index] != lef[index + 1]) {
      e.Add(Glyph(GlyphType::NUMBER, LEFDecodeNumber(lef, index)));
    } else if (lef[index]) {
      e.Add(Glyph(GlyphType::AP));
      e.Add(Glyph(GlyphType::AP));
      e.Add(Glyph(FunctionType::CONS__PAIR));
      index += 2;
    } else {
      e.Add(Glyph(FunctionType::NIL__EMPTY_LIST));
      index += 2;
    }
  }
  assert(index == lef.size());
  return e.MakeRoot();
}

std::string LEFToString(const LEF& lef) {
  std::string s(lef.size(), '0');
  for (unsigned i = 0; i < lef.size(); ++i) {
    s[i] = (lef[i] ? '1' : '0');
  }
  return s;
}

LEF StringToLEF(const std::string& s) {
  LEF lef(s.size());
  for (unsigned i = 0; i < s.size(); ++i) {
    lef[i] = (s[i] == '1');
  }
  return lef;
}
