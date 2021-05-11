#include "cell.h"
#include "game.h"
#include "strategy_wsgc_uts_1p.h"

int main_submit() {
  auto s = StrategyWSGCUTS1P::Make();
  s->SetP(1);
  Game g;
  g.ReadCells();

  // game loop
  while (1) {
    g.ReadPosition();
    auto a = s->GetAction(g);
    a.Write();
  }
  return 0;
}
