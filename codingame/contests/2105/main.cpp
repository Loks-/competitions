#include "runner.h"
#include "strategy.h"
#include "strategy_mcts.h"
#include "strategy_mcts2.h"
#include "strategy_random_action.h"
#include "strategy_wsgc_uts_1p.h"

int main() {
  Runner r;
  // r.SetStrategy0(Strategy::Make());
  r.SetStrategy0(StrategyMCTS2::Make());
  // r.SetStrategy1(Strategy::Make());
  // r.SetStrategy1(StrategyRandomAction::Make());
  r.SetStrategy1(StrategyWSGCUTS1P::Make());
  r.RunGame();
  return 0;
}
