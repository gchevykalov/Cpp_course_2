#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "include/GL/freeglut.h"

#include "GeomVector.h"

class player_t;
class ghost_t;
class fruit_t;
class static_game_object_t;

using second_t = std::chrono::duration<double, std::ratio<1>>;
using m_clock_t = std::chrono::high_resolution_clock;
using time_point_t = std::chrono::time_point<m_clock_t>;

struct game_state_t {
  std::vector<std::vector<static_game_object_t*>> map;
  std::string file;

  int windowH;
  int windowW;
  int mapH;
  int mapW;

  int level;
  int playerLives;
  int playerScore;
  int maxFoodCount;
  int foodCounter;
  int curLevFruitCount;
  int fruitCounter;

  std::vector<ghost_t*> ghosts;
  int ghostCount;
  int activeGhostCount;

  player_t* player;
  bool isplayerAlive;
  geom_vector_t playerPos;

  second_t duration;          ///< boost effect duration
  int factor;              ///< ghost eating multiplier

  std::vector<fruit_t*> fruits;
  second_t lifetime;

  game_state_t() {
    windowH = 0;
    windowW = 0;
    mapH = 0;
    mapW = 0;

    level = 1;
    playerLives = 3;
    playerScore = 0;
    maxFoodCount = 0;
    foodCounter = 0;
    curLevFruitCount = 0;
    fruitCounter = 0;

    ghostCount = 2;
    activeGhostCount = 0;

    player = nullptr;
    isplayerAlive = true;

    duration = second_t(0);
    factor = 1;

    lifetime = second_t(0);
  }
};