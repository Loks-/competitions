#include "galaxy/evaluation.h"

#include "galaxy/dictionary.h"
#include "galaxy/expression.h"
#include "galaxy/function_type.h"
#include "galaxy/glyph_type.h"
#include "galaxy/node.h"

#include "common/base.h"

#include <iostream>
#include <utility>
#include <vector>

namespace {
bool skip_draw = false;

Node* GetI(std::vector<Node*>& current_path, unsigned index) {
  return (index < current_path.size())
             ? current_path[current_path.size() - index - 1]
             : nullptr;
}

std::pair<int64_t, int64_t> GetPair(Node* node) {
  assert(IsPair(node));
  assert(node->l->r->data.type == GlyphType::NUMBER);
  assert(node->r->data.type == GlyphType::NUMBER);
  return {node->l->r->data.value, node->r->data.value};
}

unsigned ExpectedParameters(FunctionType ftype) {
  switch (ftype) {
    case FunctionType::GALAXY:
      return 0;
    case FunctionType::SUCCESSOR:
    case FunctionType::PREDECESSOR:
    case FunctionType::MODULATE:
    case FunctionType::DEMODULATE:
    case FunctionType::SEND:
    case FunctionType::NEGATE:
    case FunctionType::POWER_OF_TWO:
    case FunctionType::I_COMBINATOR:
    case FunctionType::CAR__FIRST:
    case FunctionType::CDR__TAIL:
    case FunctionType::NIL__EMPTY_LIST:
    case FunctionType::IS_NIL:
    case FunctionType::DRAW:
    case FunctionType::MULTIPLE_DRAW:
    case FunctionType::MODEM:
      return 1;
    case FunctionType::SUM:
    case FunctionType::PRODUCT:
    case FunctionType::DIVISION:
    case FunctionType::EQUALITY:
    case FunctionType::STRICT_LESS:
    case FunctionType::K_COMBINATOR:
    case FunctionType::FALSE__SECOND:
    case FunctionType::F38:
      return 2;
    case FunctionType::S_COMBINATOR:
    case FunctionType::C_COMBINATOR:
    case FunctionType::B_COMBINATOR:
    case FunctionType::CONS__PAIR:
    case FunctionType::VECTOR:
    case FunctionType::IF0:
    case FunctionType::INTERACT:
      return 3;
    default:
      return unsigned(-1);
  }
}

Node* ApplyFunction(Node* node, std::vector<Node*>& current_path) {
  assert(node && node->data.type == GlyphType::FUNCTION);
  assert(current_path.size() >= ExpectedParameters(node->data.ftype));
  Node *p0 = GetI(current_path, 0), *p1 = GetI(current_path, 1),
       *p2 = GetI(current_path, 2);
  switch (node->data.ftype) {
    case FunctionType::GALAXY:
      node->data.type = GlyphType::ALIAS;
      node->data.value = 0;
      return p0;
    case FunctionType::SUCCESSOR:
      Evaluate(p0->r);
      assert(p0->r->data.type == GlyphType::NUMBER);
      p0->data.type = GlyphType::NUMBER;
      p0->data.value = p0->r->data.value + 1;
      return p0;
    case FunctionType::PREDECESSOR:
      Evaluate(p0->r);
      assert(p0->r->data.type == GlyphType::NUMBER);
      p0->data.type = GlyphType::NUMBER;
      p0->data.value = p0->r->data.value - 1;
      return p0;
    case FunctionType::MODULATE:
      Evaluate(p0->r);
      p0->data.type = GlyphType::LINEAR_ENCODED_FORM;
      p0->data.lef = LEFEncodeNode(p0->r);
      return p0;
    case FunctionType::DEMODULATE:
      Evaluate(p0->r);
      p0->l = GetFromDictionary(FunctionType::I_COMBINATOR);
      p0->r = LEFDecodeExpression(p0->r->data.lef);
      return p0;
    case FunctionType::SEND:
      Evaluate(p0->r);
      std::cout << "[SEND] ";
      Print(p0->r);
      std::cout << std::endl;
      // Realsending is not supported yet.
      p0->l = GetFromDictionary(FunctionType::I_COMBINATOR);
      return p0;
    case FunctionType::NEGATE:
      Evaluate(p0->r);
      assert(p0->r->data.type == GlyphType::NUMBER);
      p0->data.type = GlyphType::NUMBER;
      p0->data.value = -p0->r->data.value;
      return p0;
    case FunctionType::POWER_OF_TWO:
      Evaluate(p0->r);
      assert(p0->r->data.type == GlyphType::NUMBER);
      assert(p0->r->data.value >= 0);
      p0->data.type = GlyphType::NUMBER;
      p0->data.value = (1ll << p0->r->data.value);
      return p0;
    case FunctionType::I_COMBINATOR:
      if (p1) {
        p1->l = p0->r;
        return p1;
      } else {
        Evaluate(p0->r);
        *p0 = *p0->r;
        return p0;
      }
    case FunctionType::CAR__FIRST:
    case FunctionType::CDR__TAIL:
      p0->l = p0->r;
      p0->r = GetFromDictionary((node->data.ftype == FunctionType::CAR__FIRST)
                                    ? FunctionType::K_COMBINATOR
                                    : FunctionType::FALSE__SECOND);
      return p0;
    case FunctionType::NIL__EMPTY_LIST:
      p0->data.type = GlyphType::FUNCTION;
      p0->data.ftype = FunctionType::K_COMBINATOR;
      return p0;
    case FunctionType::IS_NIL:
      EvaluateLazyIsNil(p0->r);
      if (IsNil(p0->r)) {
        p0->data.type = GlyphType::FUNCTION;
        p0->data.ftype = FunctionType::K_COMBINATOR;
      } else if (IsPair(p0->r)) {
        p0->data.type = GlyphType::FUNCTION;
        p0->data.ftype = FunctionType::FALSE__SECOND;
      } else {
        assert(false);
      }
      return p0;
    case FunctionType::DRAW:
      if (!skip_draw) {
        Evaluate(p0->r);
        assert(IsList(p0->r));
        p0->data.pic.Clear();
        for (Node* c = p0->r; !IsNil(c); c = c->r) {
          assert(c->l->data.type == GlyphType::AP);
          assert(c->l->l->data.ftype == FunctionType::CONS__PAIR);
          auto p = GetPair(c->l->r);
          p0->data.pic.AddPixel(p.first, p.second);
        }
      }
      p0->data.type = GlyphType::PICTURE;
      return p0;
    case FunctionType::MULTIPLE_DRAW: {
      Evaluate(p0->r);
      if (IsNil(p0->r)) {
        p0->l = GetFromDictionary(FunctionType::I_COMBINATOR);
      } else {
        assert(IsPair(p0->r));
        auto n1 = NewNode(GlyphType::AP);
        auto n2 = NewNode(GlyphType::AP);
        auto n3 = NewNode(GlyphType::AP);
        n1->l = GetFromDictionary(FunctionType::DRAW);
        n1->r = p0->r->l->r;
        n2->l = p0->r->l->l;
        n2->r = n1;
        n3->l = p0->l;
        n3->r = p0->r->r;
        p0->l = n2;
        p0->r = n3;
      }
      return p0;
    }
    case FunctionType::MODEM: {
      auto n1 = NewNode(GlyphType::AP);
      n1->l = GetFromDictionary(FunctionType::MODULATE);
      n1->r = p0->r;
      p0->l = GetFromDictionary(FunctionType::DEMODULATE);
      p0->r = n1;
      return p0;
    }
    case FunctionType::SUM:
      Evaluate(p0->r);
      Evaluate(p1->r);
      if ((p0->r->data.type == GlyphType::NUMBER) && (p0->r->data.value == 0)) {
        p1->data = p1->r->data;
      } else if ((p1->r->data.type == GlyphType::NUMBER) &&
                 (p1->r->data.value == 0)) {
        p1->data = p0->r->data;
      } else {
        assert(p0->r->data.type == GlyphType::NUMBER);
        assert(p1->r->data.type == GlyphType::NUMBER);
        p1->data.type = GlyphType::NUMBER;
        p1->data.value = p0->r->data.value + p1->r->data.value;
      }
      return p1;
    case FunctionType::PRODUCT:
      Evaluate(p0->r);
      if ((p0->r->data.type == GlyphType::NUMBER) && (p0->r->data.value == 0)) {
        p1->data.type = GlyphType::NUMBER;
        p1->data.value = 0;
        return p1;
      }
      Evaluate(p1->r);
      if ((p0->r->data.type == GlyphType::NUMBER) && (p0->r->data.value == 1)) {
        p1->data = p1->r->data;
      } else if ((p1->r->data.type == GlyphType::NUMBER) &&
                 (p1->r->data.value == 0)) {
        p1->data.type = GlyphType::NUMBER;
        p1->data.value = 0;
      } else if ((p1->r->data.type == GlyphType::NUMBER) &&
                 (p1->r->data.value == 1)) {
        p1->data = p0->r->data;
      } else {
        assert(p0->r->data.type == GlyphType::NUMBER);
        assert(p1->r->data.type == GlyphType::NUMBER);
        p1->data.type = GlyphType::NUMBER;
        p1->data.value = p0->r->data.value * p1->r->data.value;
      }
      return p1;
    case FunctionType::DIVISION:
      if (p0->r == p1->r) {
        p1->data.type = GlyphType::NUMBER;
        p1->data.value = 0;
        return p1;
      }
      Evaluate(p1->r);
      if ((p1->r->data.type == GlyphType::NUMBER) && (p1->r->data.value == 1)) {
        p1->data = p0->r->data;
      } else {
        Evaluate(p0->r);
        assert(p0->r->data.type == GlyphType::NUMBER);
        assert(p1->r->data.type == GlyphType::NUMBER);
        p1->data.type = GlyphType::NUMBER;
        p1->data.value = p0->r->data.value / p1->r->data.value;
      }
      return p1;
    case FunctionType::EQUALITY:
      if (p0->r == p1->r) {
        p1->data.type = GlyphType::FUNCTION;
        p1->data.ftype = FunctionType::K_COMBINATOR;
      } else {
        Evaluate(p0->r);
        Evaluate(p1->r);
        if ((p0->r->data.type == GlyphType::VARIABLE) &&
            (p1->r->data.type == GlyphType::VARIABLE) &&
            (p0->r->data.value == p1->r->data.value)) {
          p1->data.type = GlyphType::FUNCTION;
          p1->data.ftype = FunctionType::K_COMBINATOR;
        } else {
          assert(p0->r->data.type == GlyphType::NUMBER);
          assert(p1->r->data.type == GlyphType::NUMBER);
          p1->data.type = GlyphType::FUNCTION;
          p1->data.ftype = (p0->r->data.value == p1->r->data.value)
                               ? FunctionType::K_COMBINATOR
                               : FunctionType::FALSE__SECOND;
        }
      }
      return p1;
    case FunctionType::STRICT_LESS:
      if (p0->r == p1->r) {
        p1->data.type = GlyphType::FUNCTION;
        p1->data.ftype = FunctionType::FALSE__SECOND;
      } else {
        Evaluate(p0->r);
        Evaluate(p1->r);
        assert(p0->r->data.type == GlyphType::NUMBER);
        assert(p1->r->data.type == GlyphType::NUMBER);
        p1->data.type = GlyphType::FUNCTION;
        p1->data.ftype = (p0->r->data.value < p1->r->data.value)
                             ? FunctionType::K_COMBINATOR
                             : FunctionType::FALSE__SECOND;
      }
      return p1;
    case FunctionType::K_COMBINATOR:
      p1->r = p0->r;
      p1->l = GetFromDictionary(FunctionType::I_COMBINATOR);
      return p1;
    case FunctionType::FALSE__SECOND:
      p1->l = GetFromDictionary(FunctionType::I_COMBINATOR);
      return p1;
    case FunctionType::F38: {
      auto n01 = NewNode(GlyphType::AP);
      n01->l = GetFromDictionary(FunctionType::CAR__FIRST);
      n01->r = p1->r;
      auto n02 = NewNode(GlyphType::AP);
      n02->l = GetFromDictionary(FunctionType::IF0);
      n02->r = n01;
      auto n03 = NewNode(GlyphType::AP);
      n03->l = GetFromDictionary(FunctionType::CDR__TAIL);
      n03->r = p1->r;
      auto n04 = NewNode(GlyphType::AP);
      n04->l = GetFromDictionary(FunctionType::CAR__FIRST);
      n04->r = n03;
      auto n05 = NewNode(GlyphType::AP);
      n05->l = GetFromDictionary(FunctionType::MODEM);
      n05->r = n04;
      auto n06 = NewNode(GlyphType::AP);
      n06->l = GetFromDictionary(FunctionType::CONS__PAIR);
      n06->r = n05;
      auto n07 = NewNode(GlyphType::AP);
      n07->l = GetFromDictionary(FunctionType::CDR__TAIL);
      n07->r = n03;
      auto n08 = NewNode(GlyphType::AP);
      n08->l = GetFromDictionary(FunctionType::CAR__FIRST);
      n08->r = n07;
      auto n09 = NewNode(GlyphType::AP);
      n09->l = GetFromDictionary(FunctionType::MULTIPLE_DRAW);
      n09->r = n08;
      auto n10 = NewNode(GlyphType::AP);
      n10->l = GetFromDictionary(FunctionType::CONS__PAIR);
      n10->r = n09;
      auto n11 = NewNode(GlyphType::AP);
      n11->l = n10;
      n11->r = GetFromDictionary(FunctionType::NIL__EMPTY_LIST);
      auto n12 = NewNode(GlyphType::AP);
      n12->l = n06;
      n12->r = n11;
      auto n13 = NewNode(GlyphType::AP);
      n13->l = n02;
      n13->r = n12;
      auto n14 = NewNode(GlyphType::AP);
      n14->l = GetFromDictionary(FunctionType::INTERACT);
      n14->r = p0->r;
      auto n15 = NewNode(GlyphType::AP);
      n15->l = n14;
      n15->r = n05;
      auto n16 = NewNode(GlyphType::AP);
      n16->l = GetFromDictionary(FunctionType::SEND);
      n16->r = n08;
      auto n17 = NewNode(GlyphType::AP);
      n17->l = n15;
      n17->r = n16;
      p1->l = n13;
      p1->r = n17;
      return p1;
    };
    case FunctionType::S_COMBINATOR: {
      auto n1 = NewNode(GlyphType::AP);
      auto n2 = NewNode(GlyphType::AP);
      n1->l = p0->r;
      n1->r = p2->r;
      n2->l = p1->r;
      n2->r = p2->r;
      p2->l = n1;
      p2->r = n2;
      return p2;
    }
    case FunctionType::C_COMBINATOR: {
      auto n1 = NewNode(GlyphType::AP);
      n1->l = p0->r;
      n1->r = p2->r;
      p2->l = n1;
      p2->r = p1->r;
      return p2;
    }
    case FunctionType::B_COMBINATOR: {
      auto n1 = NewNode(GlyphType::AP);
      n1->l = p1->r;
      n1->r = p2->r;
      p2->l = p0->r;
      p2->r = n1;
      return p2;
    }
    case FunctionType::CONS__PAIR:
    case FunctionType::VECTOR: {
      auto n1 = NewNode(GlyphType::AP);
      n1->l = p2->r;
      n1->r = p0->r;
      p2->l = n1;
      p2->r = p1->r;
      return p2;
    }
    case FunctionType::IF0:
      Evaluate(p0->r);
      assert(p0->r->data.type == GlyphType::NUMBER);
      if (p0->r->data.value == 0) p2->r = p1->r;
      p2->l = GetFromDictionary(FunctionType::I_COMBINATOR);
      return p2;
    case FunctionType::INTERACT: {
      auto n1 = NewNode(GlyphType::AP);
      auto n2 = NewNode(GlyphType::AP);
      auto n3 = NewNode(GlyphType::AP);
      n1->l = GetFromDictionary(FunctionType::F38);
      n1->r = p0->r;
      n2->l = p0->r;
      n2->r = p1->r;
      n3->l = n2;
      n3->r = p2->r;
      p2->l = n1;
      p2->r = n3;
      return p2;
    }
    default:
      assert(false);
      return nullptr;
  }
}

Node* EvaluateI(Node* node, std::vector<Node*>& current_path) {
  bool subtree_changed = false;
  while (true) {
    assert(node);
    if (node->data.type == GlyphType::ALIAS) {
      subtree_changed = true;
      ExpandAlias(node);
    } else if (node->data.type == GlyphType::AP) {
      current_path.push_back(node);
      Node* pnext = EvaluateI(node->l, current_path);
      current_path.pop_back();
      if (pnext == node) {
        subtree_changed = true;
      } else if (!pnext) {
        Evaluate(node->r);
        break;
      } else {
        return pnext;
      }
    } else if (node->data.type == GlyphType::FUNCTION) {
      if (current_path.size() >= ExpectedParameters(node->data.ftype)) {
        subtree_changed = true;
        Node* pnext = ApplyFunction(node, current_path);
        if (pnext != node) return pnext;
      } else {
        break;
      }
    } else {
      break;
    }
  }
  return subtree_changed ? GetI(current_path, 0) : nullptr;
}
}  // namespace

void EvaluationSkipDraw(bool _skip_draw) { skip_draw = _skip_draw; }

void ExpandAlias(Node* node) {
  assert(node && node->data.type == GlyphType::ALIAS);
  node->l = GetFromDictionary(FunctionType::I_COMBINATOR);
  node->r = GetFromDictionary(node->data.value);
  node->data.type = GlyphType::AP;
}

void EvaluateLazyIsNil(Node* node) {
  assert(node);
  std::vector<Node*> v;
  v.push_back(node);
  for (;;) {
    if (node->data.type == GlyphType::ALIAS) {
      ExpandAlias(node);
    } else if (node->data.type == GlyphType::AP) {
      auto l = node->l;
      if (l->data.type == GlyphType::ALIAS) {
        ExpandAlias(l);
      } else if (l->data.type == GlyphType::FUNCTION) {
        if (ExpectedParameters(l->data.ftype) <= 1) {
          ApplyFunction(l, v);
        } else {
          break;
        }
      } else if (l->data.type == GlyphType::AP) {
        auto l2 = l->l;
        if (l2->data.type == GlyphType::ALIAS) {
          ExpandAlias(l2);
        } else if (l2->data.type == GlyphType::FUNCTION) {
          if (ExpectedParameters(l2->data.ftype) <= 2) {
            v.push_back(l);
            ApplyFunction(l2, v);
            v.pop_back();
          } else {
            break;
          }
        } else if (l2->data.type == GlyphType::AP) {
          v.push_back(l);
          EvaluateI(l2, v);
          v.pop_back();
        } else {
          break;
        }
      } else {
        break;
      }
    } else {
      break;
    }
  }
}

void Evaluate(Node* node) {
  assert(node);
  std::vector<Node*> v;
  EvaluateI(node, v);
  assert(v.size() == 0);
}
