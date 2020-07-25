#include "tournament/message.h"

#include <iostream>
#include <string>

namespace tournament {
void Print(const Message& m) {
  if (m.IsValue()) {
    std::cout << ((m.Value() == PAIR_CODE) ? "P" : std::to_string(m.Value()))
              << " ";
  } else {
    std::cout << "( ";
    for (auto& it : m.Vector()) Print(it);
    std::cout << ") ";
  }
}
}  // namespace tournament
