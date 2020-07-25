#include "tournament/encoder.h"

#include "app/send.h"
#include "tournament/message.h"

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

namespace tournament {
namespace {
void EncodeMessageI(const Message& m, std::ostream& s) {
  if (m.IsValue()) {
    int64_t value = m.Value(), avalue = abs(value);
    s << ((value < 0) ? "10" : "01");
    unsigned l = 0;
    for (; (1ll << (4 * l)) <= avalue;) {
      ++l;
      s << "1";
    }
    s << "0";
    for (unsigned i = 0; i < 4 * l; ++i) {
      s << ((avalue & (1ll << (4 * l - i - 1)) ? "1" : "0"));
    }
  } else if ((m.Size() == 3) && (m[0].IsValue()) &&
             (m[0].Value() == PAIR_CODE)) {
    s << "11";
    EncodeMessageI(m[1], s);
    EncodeMessageI(m[2], s);
  } else {
    for (auto& it : m.Vector()) {
      s << "11";
      EncodeMessageI(it, s);
    }
    s << "00";
  }
}
};  // namespace

std::string EncodeMessage(const Message& m) {
  std::ostringstream ss;
  EncodeMessageI(m, ss);
  return ss.str();
}

Message DecodeMessage(const std::string& /* s */) { return {}; }

Message Send(const Message& m) {
  std::cout << "[SEND] ";
  Print(m);
  std::cout << std::endl;
  auto s = EncodeMessage(m);
  auto r = app::SendRaw(s);
  auto dm = DecodeMessage(r);
  std::cout << "[RECEIVED] ";
  Print(dm);
  std::cout << std::endl;
  return dm;
}
}  // namespace tournament
