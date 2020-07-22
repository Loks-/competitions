#include "node.h"

#include "function_type.h"
#include "glyph_decoder.h"
#include "glyph_type.h"

#include "common/nodes_manager.h"

#include <iostream>

namespace {
NodesManager<Node> manager(1000);

void PrintI(Node* node, GlyphDecoder& gd);

void PrintListI(Node* node, bool first, GlyphDecoder& gd) {
  if (IsNil(node)) {
    std::cout << (first ? "( " : "") << ") ";
  } else {
    assert(IsPair(node));
    std::cout << (first ? "(" : ", ");
    PrintI(node->l->r, gd);
    PrintListI(node->r, false, gd);
  }
}

void PrintI(Node* node, GlyphDecoder& gd) {
  assert(node);
  if (IsList(node)) {
    PrintListI(node, true, gd);
  } else {
    std::cout << gd.ToString(node->data) << " ";
    if (node->data.type == GlyphType::AP) {
      PrintI(node->l, gd);
      PrintI(node->r, gd);
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

void Print(Node* node) {
  auto& gd = GlyphDecoder::GetDecoder();
  PrintI(node, gd);
}
