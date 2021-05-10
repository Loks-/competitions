#include "runner.h"
#include "strategy.h"
#include "strategy_mcts.h"
#include "strategy_mcts2.h"
#include "strategy_random_action.h"

int main() {
  Runner r;
  // r.SetStrategy0(Strategy::Make());
  r.SetStrategy0(StrategyMCTS2::Make());
  // r.SetStrategy1(StrategyRandomAction::Make());
  r.SetStrategy1(StrategyMCTS::Make());
  r.RunGame();
  return 0;
}
