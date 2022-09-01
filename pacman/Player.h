#pragma once

#include "Unit.h"

class player_t : public unit_t {
public:
  player_t(int x, int y, game_state_t* state) : unit_t(x, y, state) {};

  void changeDirection(int key);

  virtual void move(void) override;
  virtual void respawn(void) override;
  virtual void draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) override;
};