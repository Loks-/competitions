#pragma once

#include "common/base.h"

#include <string>
#include <vector>

class Node;

using LEF = std::vector<bool>;

LEF LEFEncodeNumber(int64_t value);
LEF LEFEncodeNIL();
LEF LEFEncodeCons();
LEF LEFEncodePair(Node* node);
LEF LEFEncodeNode(Node* node);

int64_t LEFDecodeNumber(const LEF& lef, unsigned& index);
int64_t LEFDecodeNumber(const LEF& lef);
Node* LEFDecodeExpression(const LEF& lef);

std::string LEFToString(const LEF& lef);
LEF StringToLEF(const std::string& s);
