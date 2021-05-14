#include "fstrategy_builder.h"
#include "fstrategy_builder2.h"
#include "fstrategy_complete.h"
#include "fstrategy_wait_and_complete.h"
#include "runner.h"
#include "strategy.h"
#include "strategy_mcts.h"
#include "strategy_mcts2.h"
#include "strategy_mcts3.h"
#include "strategy_mcts4.h"
#include "strategy_proxy.h"
#include "strategy_proxy.h"
#include "strategy_wsgc_mcts.h"
#include "strategy_wsgc_mcts2.h"
#include "strategy_wsgc_uts.h"
#include "strategy_wsgc_uts2.h"

#include <string>

// using TStrategy0 = StrategyProxy<FStrategyBuilder<1, 1>>;
// using TStrategy0 = StrategyProxy<FStrategyBuilder2<1, 1, 2, 4>>;
// using TStrategy0 = StrategyMCTS3<FStrategyBuilder<>>;
// using TStrategy0 = StrategyMCTS4<FStrategyBuilder<>>;
using TStrategy0 = StrategyWSGCMCTS<FStrategyBuilder<>>;

// using TStrategy1 = StrategyProxy<FStrategyBuilder<>>;
// using TStrategy1 = StrategyProxy<FStrategyBuilder2<>>;
// using TStrategy1 = StrategyMCTS3<FStrategyBuilder<>>;
// using TStrategy1 = StrategyMCTS3<FStrategyBuilder2<>>;
// using TStrategy1 = StrategyMCTS4<FStrategyBuilder<>>;
// using TStrategy1 = StrategyWSGCMCTS<FStrategyBuilder<>>;
// using TStrategy1 = StrategyWSGCMCTS<FStrategyBuilder2<>>;
using TStrategy1 = StrategyWSGCMCTS2<FStrategyBuilder<>>;

int main(int nargs, char **pargs) {
  if (nargs > 1) {
    unsigned ngames = std::stoul(pargs[1]);
    Runner::RunTP<TStrategy0, TStrategy1>(ngames);
  } else {
    Runner r;
    r.SetStrategy0(TStrategy0::Make());
    r.SetStrategy1(TStrategy1::Make());
    r.RunGame();
    // r.RunNGames(40);
  }
  return 0;
}
