#include "cell.h"
#include "game.h"

#include "common/base.h"
#include "common/stl/base.h"

int main() {
  Game g;
  g.ReadCells();

  // game loop
  while (1) {
    g.ReadPosition();
    auto m = g.GetPossibleActions(1);
    auto a = m[rand() % m.size()];
    a.Write();
  }
  return 0;
}
