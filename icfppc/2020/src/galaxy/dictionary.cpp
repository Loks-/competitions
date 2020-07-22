#include "galaxy/dictionary.h"

#include "galaxy/node.h"

#include "common/base.h"
#include "common/template.h"

#include <unordered_map>

namespace {
std::unordered_map<unsigned, Node*> dict;
std::unordered_map<FunctionType, Node*> fdict;
}  // namespace

void AddToDictionary(unsigned index, Node* node) {
  auto it = dict.find(index);
  FakeUse(it);
  assert(it == dict.end());
  dict[index] = node;
}

Node* GetFromDictionary(unsigned index) {
  auto it = dict.find(index);
  assert(it != dict.end());
  return it->second;
}

Node* GetFromDictionary(FunctionType ftype) {
  auto it = fdict.find(ftype);
  if (it == fdict.end())
    return fdict[ftype] = NewNode(Glyph(ftype));
  else
    return it->second;
}
