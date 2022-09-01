#pragma once

#include "GameState.h"

class game_object_t {
protected:
  geom_vector_t pos; ///< object position on the map
  game_state_t* const gameState;
public:
  game_object_t(int x, int y, game_state_t* state) : pos(x, y), gameState(state) {};

  geom_vector_t getPos(void) {
    return pos;
  }

  // draw an object inside the specified area
  virtual void draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) = 0;
};