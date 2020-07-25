#include "app/encoder.h"

#include "galaxy/linear_encoded_form.h"
#include "galaxy/message_decoder.h"
#include "galaxy/node.h"

#include <iostream>

std::string EncodeMessage(const std::string& s) {
  MessageDecoder md;
  auto e = md.DecodeExpression(s);
  auto lef = LEFEncodeNode(e.MakeRoot());
  return LEFToString(lef);
}

std::string DecodeMessage(const std::string& s) {
  if (s.empty()) return s;

  auto lef = StringToLEF(s);
  Node* node = LEFDecodeExpression(lef);
  std::cout << "[RECEIVED] ";
  Print(node);
  std::cout << std::endl;

  return s;
}
