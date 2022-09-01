#pragma once

#include "GameObjects.h"
#include <map>

class unit_t : public game_object_t {
public:
  enum class direction_t {
    Up,
    Down,
    Right,
    Left
  };
protected:
  static const std::map<unit_t::direction_t, geom_vector_t> dirVecs;

  const geom_vector_t startPos;
  unit_t::direction_t curDirection;
public:
  unit_t(int x, int y, game_state_t* state) : game_object_t(x, y, state), startPos(x, y),
    curDirection(direction_t::Up) {};

  virtual void respawn(void) = 0;
  virtual void move(void) = 0;
};