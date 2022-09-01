#pragma once

#include "Unit.h"

class ghost_t : public unit_t {
public:
  enum class state_t {
    Chase,
    Runaway,
    Panic
  };
private:
  state_t state;
  int r;               ///< ghost color
  int g;               //
  int b;               //
  bool isActive;

  static const int points = 100;              ///< the number of points received by the player when eaten
  static const std::map<direction_t, std::vector<direction_t>> possibleDirs;
  
  const geom_vector_t outdoorPos; ///< spawn position outside the base
  const geom_vector_t checkPoint; ///< target to runaway
public:
  ghost_t(int x, int y, game_state_t* gameState, int red, int green, int blue, int chX, int chY, geom_vector_t door) :
    unit_t(x, y, gameState), state(state_t::Runaway), r(red), g(green), b(blue), isActive(false), checkPoint(chX, chY),
    outdoorPos(door) {};

  bool activate(void);
  void changeState(state_t s);
  bool collision(void);

  virtual void move(void) override;
  virtual void respawn(void) override;
  virtual void draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) override;
};