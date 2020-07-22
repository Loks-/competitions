#include "expression.h"

#include "evaluation.h"
#include "glyph.h"
#include "glyph_decoder.h"
#include "glyph_type.h"
#include "node.h"

#include "common/base.h"
#include "common/binary_search_tree/base/right.h"

#include <iostream>
#include <vector>

namespace {
void MakeVectorI(Node* node, std::vector<Glyph>& v) {
  assert(node);
  v.push_back(node->data);
  if (node->data.type == GlyphType::AP) {
    MakeVectorI(node->l, v);
    MakeVectorI(node->r, v);
  }
}
};  // namespace

Expression::Expression() {}

Expression::Expression(const Glyph& g) : v(1, g) {}

Node* Expression::MakeNodeI(unsigned& index) {
  assert(index < v.size());
  Node* node = NewNode(v[index++]);
  if (node->data.type == GlyphType::AP) {
    node->l = MakeNodeI(index);
    node->r = MakeNodeI(index);
  }
  return node;
}

void Expression::Add(const Glyph& g) {
  assert(!root);
  v.push_back(g);
}

Node* Expression::MakeRoot() {
  unsigned index = 0;
  root = MakeNodeI(index);
  assert(index == v.size());
  return root;
}

void Expression::MakeVector() {
  v.clear();
  MakeVectorI(root, v);
}

void Expression::Evaluate() {
  if (!root) MakeRoot();
  ::Evaluate(root);
  MakeVector();
}

void Expression::Print() {
  if (!root) MakeRoot();
  ::Print(root);
}

bool Expression::operator==(const Expression& r) const { return v == r.v; }
