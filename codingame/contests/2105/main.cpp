#include "runner.h"
#include "strategy.h"
#include "strategy_random_action.h"

int main() {
  Runner r;
  r.SetStrategy0(Strategy::Make());
  r.SetStrategy1(StrategyRandomAction::Make());
  // r.SetStrategy1(StrategyRandomAction());
  r.RunGame();
  return 0;
}
