#pragma once

#include "action_type.h"

#include "common/base.h"

#include <iostream>
#include <vector>

class Action {
 public:
  ActionType type;
  uint8_t value1, value2;

  Action(ActionType _type = WAIT, uint8_t _value1 = 0, uint8_t _value2 = 0)
      : type(_type), value1(_value1), value2(_value2) {}

  void Write() const {
    switch (type) {
      case SEED:
        std::cout << "SEED " << value1 << " " << value2 << std::endl;
        break;
      case GROW:
        std::cout << "GROW " << value1 << std::endl;
        break;
      case COMPLETE:
        std::cout << "COMPLETE " << value1 << std::endl;
        break;
      case WAIT:
        std::cout << "WAIT" << std::endl;
        break;
      default:
        assert(false);
    }
  }
};

using Actions = std::vector<Action>;
