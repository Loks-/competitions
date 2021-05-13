#include "cell.h"
#include "fstrategy_builder.h"
#include "game.h"
#include "strategy_proxy.h"
// #include "strategy_mcts3.h"
#include "strategy_wsgc_mcts.h"
// #include "strategy_wsgc_uts2.h"

int main_submit() {
  // auto s = StrategyProxy<FStrategyBuilder<>>::Make();
  // auto s = StrategyMCTS3<FStrategyBuilder<>>::Make();
  auto s = StrategyWSGCMCTS<FStrategyBuilder<>>::Make();
  // auto s = StrategyWSGCUTS2<FStrategyBuilder<>>::Make();
  s->SetP(1);
  Game g;
  g.ReadCells();
  s->Reset(g.cells);

  // game loop
  while (1) {
    g.ReadPosition();
    auto a = s->GetAction(g);
    a.Write();
  }
  return 0;
}
