#pragma once

#include "GameObjects.h"

class static_game_object_t : public game_object_t {
public:
  static_game_object_t(int x, int y, game_state_t* state) : game_object_t(x, y, state) {};

  virtual bool allowMove(void) = 0;
  virtual void eaten(void) = 0;
};


class wall_t : public static_game_object_t {
public:
  wall_t(int x, int y, game_state_t* state) : static_game_object_t(x, y, state) {};

  virtual bool allowMove(void) override;
  virtual void eaten(void) override;
  virtual void draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) override;
};


class ghost_door_t : public static_game_object_t {
public:
  ghost_door_t(int x, int y, game_state_t* state) : static_game_object_t(x, y, state) {};

  virtual bool allowMove(void) override;
  virtual void eaten(void) override;
  virtual void draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) override;
};


class empty_t : public static_game_object_t {
public:
  empty_t(int x, int y, game_state_t* state) : static_game_object_t(x, y, state) {};

  virtual bool allowMove(void) override;
  virtual void eaten(void) override;
  virtual void draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) override;
};


class food_t : public static_game_object_t {
private:
  static const int points = 10;

  bool isActive;
public:
  food_t(int x, int y, game_state_t* state) : static_game_object_t(x, y, state), isActive(true) {};

  virtual bool allowMove(void) override;
  virtual void eaten(void) override;
  virtual void draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) override;
};


class booster_t : public static_game_object_t {
private:
  static const int points = 50;
  static const second_t duration;

  bool isActive;
public:
  booster_t(int x, int y, game_state_t* state) : static_game_object_t(x, y, state), isActive(true) {};

  virtual bool allowMove(void) override;
  virtual void eaten(void) override;
  virtual void draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) override;
};


class fruit_t : public static_game_object_t {
private:
  static const int points = 100;
  static const second_t lifetime;

  bool isActive;
public:
  fruit_t(int x, int y, game_state_t* state) : static_game_object_t(x, y, state), isActive(false) {};
  
  void set(void);
  void remove(void);
  
  virtual bool allowMove(void) override;
  virtual void eaten(void) override;
  virtual void draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) override;
};