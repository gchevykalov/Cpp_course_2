#pragma once

#include "GameState.h"
#include "Ghost.h"
#include "Player.h"
#include "StaticGameObjects.h"

constexpr unsigned FONTHEIGHT = 18; //GLUT_BITMAP_HELVETICA_18
constexpr unsigned BOUNDARES = 5;

class game_t {
private:
  game_state_t state;

  game_t() {}

  void _clear(void);
  void _newLevel(void);
  void _newGame(void);
  void _checkCollision(void);
public:
  static game_t* getInst(void) {
    static game_t instance;

    return &instance;
  }

  void loadMap(std::string filename);
  void draw(void);
  void clickHandler(unsigned key);
  void move(void);
  void reshape(int W, int H);
  void end(void);
};