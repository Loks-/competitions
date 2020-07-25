#pragma once

#include "galaxy/function_type.h"

namespace galaxy {
class Node;

void AddToDictionary(unsigned index, Node* node);
Node* GetFromDictionary(unsigned index);
Node* GetFromDictionary(FunctionType ftype);
}  // namespace galaxy
