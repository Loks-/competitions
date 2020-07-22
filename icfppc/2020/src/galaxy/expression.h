#pragma once

#include "galaxy/glyph.h"

#include <vector>

class Node;

class Expression {
 public:
  std::vector<Glyph> v;
  Node* root = nullptr;

  Expression();
  explicit Expression(const Glyph& g);

 protected:
  Node* MakeNodeI(unsigned& index);

 public:
  void Add(const Glyph& g);
  Node* MakeRoot();
  void MakeVector();
  void Evaluate();
  void Print();

  bool operator==(const Expression& r) const;
  bool operator!=(const Expression& r) const;
};
