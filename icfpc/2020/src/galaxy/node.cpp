#include "galaxy/node.h"

#include "galaxy/function_type.h"
#include "galaxy/glyph_decoder.h"
#include "galaxy/glyph_type.h"

#include "common/nodes_manager.h"

#include <iostream>

namespace galaxy {
namespace {
NodesManager<Node> manager(1000);

void PrintI(Node* node, GlyphDecoder& gd, std::ostream& s);

void PrintListI(Node* node, bool first, GlyphDecoder& gd, std::ostream& s) {
  if (IsNil(node)) {
    s << (first ? "( " : "") << ") ";
  } else {
    assert(IsPair(node));
    s << (first ? "( " : ", ");
    PrintI(node->l->r, gd, s);
    PrintListI(node->r, false, gd, s);
  }
}

void PrintI(Node* node, GlyphDecoder& gd, std::ostream& s) {
  assert(node);
  if (IsList(node)) {
    PrintListI(node, true, gd, s);
  } else {
    s << gd.ToString(node->data) << " ";
    if (node->data.type == GlyphType::AP) {
      PrintI(node->l, gd, s);
      PrintI(node->r, gd, s);
    }
  }
}
}  // namespace

Node* NewNode(const Glyph& g) {
  auto node = manager.New();
  node->data = g;
  node->l = node->r = nullptr;
  return node;
}

bool IsNil(Node* node) {
  assert(node);
  return (node->data.type == GlyphType::FUNCTION) &&
         (node->data.ftype == FunctionType::NIL__EMPTY_LIST);
}

bool IsPair(Node* node) {
  assert(node);
  return (node->data.type == GlyphType::AP) &&
         (node->l->data.type == GlyphType::AP) &&
         (node->l->l->data.type == GlyphType::FUNCTION) &&
         (node->l->l->data.ftype == FunctionType::CONS__PAIR);
}

bool IsPairOrNil(Node* node) { return IsNil(node) || IsPair(node); }

bool IsList(Node* node) {
  for (; node;) {
    if (IsNil(node)) return true;
    if (!IsPair(node)) return false;
    node = node->r;
  }
  assert(false);
  return false;
}

void Print(Node* node) { Print(node, std::cout); }

void Print(Node* node, std::ostream& s) {
  auto& gd = GlyphDecoder::GetDecoder();
  PrintI(node, gd, s);
}

}  // namespace galaxy
