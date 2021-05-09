#include "cell.h"
#include "game_turn.h"

#include "common/base.h"
#include "common/stl/base.h"

int main() {
  auto cells = Cell::ReadCells();
  GameTurn gturn;

  // game loop
  while (1) {
    gturn.Read();
    auto m = gturn.GetPossibleActions(1);
    auto a = m[rand() % m.size()];
    a.Write();
  }
  return 0;
}
