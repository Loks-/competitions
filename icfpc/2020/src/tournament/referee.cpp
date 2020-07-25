#include "tournament/referee.h"

#include "tournament/encoder.h"
#include "tournament/message.h"

#include <vector>

namespace tournament {
int Referee::Play() {
  auto initial_message = Message(std::vector<int64_t>{1, 0});
  auto initial_response = Send(initial_message);
  return 0;
}
}  // namespace tournament
