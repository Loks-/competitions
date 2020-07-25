#include "app/encoder.h"

#include "galaxy/linear_encoded_form.h"
#include "galaxy/message_decoder.h"
#include "galaxy/node.h"

#include <iostream>

namespace app {
std::string EncodeMessage(const std::string& s) {
  galaxy::MessageDecoder md;
  auto e = md.DecodeExpression(s);
  auto lef = LEFEncodeNode(e.MakeRoot());
  return galaxy::LEFToString(lef);
}

std::string DecodeMessage(const std::string& s) {
  if (s.empty()) return s;

  auto lef = galaxy::StringToLEF(s);
  auto node = galaxy::LEFDecodeExpression(lef);
  std::cout << "[RECEIVED] ";
  galaxy::Print(node);
  std::cout << std::endl;

  return s;
}
}  // namespace app
