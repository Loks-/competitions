#pragma once

#include "galaxy/function_type.h"

class Node;

void AddToDictionary(unsigned index, Node* node);
Node* GetFromDictionary(unsigned index);
Node* GetFromDictionary(FunctionType ftype);
