#pragma once

#include "galaxy/glyph.h"

#include "common/node.h"

class Node : public BaseNode {
 public:
  Glyph data;
  Node *l, *r;

  void ApplyAction() {}
};

Node* NewNode(const Glyph& g);

// Expression should be evaluated
bool IsNil(Node* node);
bool IsPair(Node* node);
bool IsPairOrNil(Node* node);
bool IsList(Node* node);

void Print(Node* node);
