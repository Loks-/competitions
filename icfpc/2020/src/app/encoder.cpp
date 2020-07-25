#include "app/encoder.h"

#include "galaxy/linear_encoded_form.h"
#include "galaxy/message_decoder.h"
#include "galaxy/node.h"

#include <sstream>
#include <string>

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
  std::ostringstream ss;
  galaxy::Print(node, ss);
  return ss.str();
}
}  // namespace app
