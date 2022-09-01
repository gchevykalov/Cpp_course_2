#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cstdlib>

void game_t::_clear(void) {
  delete state.player;
  for (int i = 0; i < state.ghosts.size(); ++i)
    delete state.ghosts[i];
  state.ghosts.clear();
  for (int i = 0; i < state.fruits.size(); ++i)
    delete state.fruits[i];
  state.fruits.clear();
  for (auto& v : state.map) {
    for (int i = 0; i < v.size(); ++i)
      delete v[i];
  }
  state.map.clear();
}

void game_t::loadMap(std::string filename) {
  std::ifstream iStream(filename);

  if (iStream.is_open() == 0) {
    std::cerr << "ERROR: Incorrect file name" << std::endl;
    this->_clear();
    exit(0);
  }
  if (iStream.eof() == true) {
    std::cerr << "ERROR: Empty file" << std::endl;
    this->_clear();
    exit(0);
  }

  this->_clear();

  state.file = filename;
  std::string line;
  int j = 0;
  geom_vector_t door(1, 1);
  while (std::getline(iStream, line)) {
    if (state.mapW == 0)
      state.mapW = line.length();
    if (line.length() != state.mapW) {
      std::cerr << "ERROR: Wrong line length" << std::endl;
      this->_clear();
      exit(0);
    }

    state.map.push_back(std::vector<static_game_object_t*>());
    for (int i = 0; i < state.mapW; ++i) {
      switch (line[i]) {
      case '#': //wall
        state.map[state.map.size() - 1].push_back(new wall_t(i, j, &state));
        break;
      case '_': //door
        door = geom_vector_t(i, j - 1);
        state.map[state.map.size() - 1].push_back(new ghost_door_t(i, j, &state));
        break;
      case ' ': //empty
        state.map[state.map.size() - 1].push_back(new empty_t(i, j, &state));
        break;
      case '.': //food
        state.map[state.map.size() - 1].push_back(new food_t(i, j, &state));
        state.maxFoodCount++;
        break;
      case '*': //booster
        state.map[state.map.size() - 1].push_back(new booster_t(i, j, &state));
        break;
      case '&': //fruit
        state.fruits.push_back(new fruit_t(i, j, &state));
        state.map[state.map.size() - 1].push_back(new food_t(i, j, &state));
        state.maxFoodCount++;
        break;
      case 'r': //red ghost
        if (state.ghosts.size() < 4) {
          state.ghosts.push_back(new ghost_t(i, j, &state, 255, 0, 0, rand() % (state.mapW / 2), -1, door));
        }
        state.map[state.map.size() - 1].push_back(new empty_t(i, j, &state));
        break;
      case 'p': //pink ghost
        if (state.ghosts.size() < 4) {
          state.ghosts.push_back(new ghost_t(i, j, &state, 255, 192, 203, -1, rand() % (4 * j), door));
        }
        state.map[state.map.size() - 1].push_back(new empty_t(i, j, &state));
        break;
      case 'b': //blue ghost
        if (state.ghosts.size() < 4) {
          state.ghosts.push_back(new ghost_t(i, j, &state, 0, 0, 255, state.mapW / 2 + rand() % (state.mapW / 2), -1, door));
        }
        state.map[state.map.size() - 1].push_back(new empty_t(i, j, &state));
        break;
      case 'o': //orange ghost
        if (state.ghosts.size() < 4) {
          state.ghosts.push_back(new ghost_t(i, j, &state, 255, 136, 0, rand() % state.mapW, rand() % (4 * j), door));
        }
        state.map[state.map.size() - 1].push_back(new empty_t(i, j, &state));
        break;
      case '@': //pacman
        state.player = new player_t(i, j, &state);
        state.map[state.map.size() - 1].push_back(new empty_t(i, j, &state));
        break;
      default: //smth other
        state.map[state.map.size() - 1].push_back(new empty_t(i, j, &state));
        break;
      }
    }
    j++;
  }
  state.mapH = j;
}

void game_t::clickHandler(unsigned key) {
  if (key == 27) {
    this->_clear();
    exit(0);
  }
  state.player->changeDirection(key);
}

void game_t::reshape(int W, int H) {
  state.windowW = W;
  state.windowH = H;
}

void game_t::draw(void) {
  geom_vector_t upperLeft(BOUNDARES, 2 * BOUNDARES + FONTHEIGHT);
  geom_vector_t lowerRight(state.windowW - BOUNDARES, state.windowH - 4 * BOUNDARES - 2 * FONTHEIGHT);

  for (auto& s : state.map)
    for (auto c : s)
      c->draw(upperLeft, lowerRight);

  for (auto f : state.fruits)
    f->draw(upperLeft, lowerRight);

  state.player->draw(upperLeft, lowerRight);

  for (auto g : state.ghosts)
    g->draw(upperLeft, lowerRight);

  glColor3d(1, 1, 1);
  std::string str = "Level: " + std::to_string(state.level) + "  Score: " + std::to_string(state.playerScore);
  glRasterPos2d(BOUNDARES, glutGet(GLUT_WINDOW_HEIGHT) - BOUNDARES - FONTHEIGHT);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)str.c_str());

  str = "Lives: " + std::to_string(state.playerLives);
  glRasterPos2d(BOUNDARES, BOUNDARES);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)str.c_str());

  glColor3d(0, 1, 0);
  int x = glutGet(GLUT_WINDOW_WIDTH) - BOUNDARES - FONTHEIGHT;
  glRecti(glutGet(GLUT_WINDOW_WIDTH) - BOUNDARES, BOUNDARES, x, BOUNDARES + FONTHEIGHT);

  glColor3d(1, 1, 1);
  str = std::to_string(state.fruitCounter) + " ";
  glRasterPos2d(x - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)str.c_str()), BOUNDARES);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)str.c_str());
}

void game_t::end(void) {
  this->_clear();
}

void game_t::_newLevel(void) {
  state.level++;
  state.curLevFruitCount = 0;
  state.foodCounter = 0;
  state.activeGhostCount = 0;
  state.ghostCount = 2;
  this->loadMap(state.file);
}

void game_t::_newGame(void) {
  state.level = 1;
  state.playerLives = 3;
  state.playerScore = 0;
  state.maxFoodCount = 0;
  state.foodCounter = 0;
  state.curLevFruitCount = 0;
  state.fruitCounter = 0;

  state.ghostCount = 2;
  state.activeGhostCount = 0;

  state.isplayerAlive = true;
  state.duration = second_t(0);
  state.factor = 1;
  state.lifetime = second_t(0);

  loadMap(state.file);
}

void game_t::_checkCollision(void) {
  geom_vector_t pPos = state.player->getPos();

  for (auto g : state.ghosts)
    if (g->getPos() == pPos)
      if (g->collision())
        return;

  for (auto f : state.fruits)
    if (f->getPos() == pPos) {
      f->eaten();
      return;
    }
}

void game_t::move(void) {
  static time_point_t t1, t2;
  static const second_t period(10);
  static second_t beforeChange = period;
  static ghost_t::state_t s = ghost_t::state_t::Runaway;
  second_t dur(0);

  t1 = m_clock_t::now();

  if (t2 != time_point_t())
    dur = std::chrono::duration_cast<second_t>(t1 - t2);

  if (state.foodCounter == state.maxFoodCount)
    this->_newLevel();

  if (state.playerLives == 0)
    this->_newGame();

  if (!state.isplayerAlive) {
    state.player->respawn();
    for (auto g : state.ghosts)
      g->respawn();
  }

  if (state.foodCounter == state.maxFoodCount / 3 ||
    state.foodCounter == state.maxFoodCount * 2 / 3) {
    state.ghostCount++;
    for (auto f : state.fruits)
      f->set();
  }

  if (state.activeGhostCount < state.ghostCount) {
    for (auto g : state.ghosts)
      if (g->activate())
        break;
  }

  for (auto g : state.ghosts)
    g->move();

  this->_checkCollision();

  if (state.isplayerAlive) {
    state.player->move();
    this->_checkCollision();
  }

  if (state.lifetime > second_t(0)) {
    state.lifetime -= dur;
    if (state.lifetime <= second_t(0))
      for (auto f : state.fruits)
        f->remove();
  }

  if (state.duration > second_t(0)) {
    state.duration -= dur;
    if (state.duration <= second_t(0))
      for (auto g : state.ghosts)
        g->changeState(ghost_t::state_t::Chase);
  }
  else if (beforeChange > second_t(0)) {
    beforeChange -= dur;
    if (beforeChange <= second_t(0)) {
      beforeChange = period;
      if (s == ghost_t::state_t::Runaway) {
        s = ghost_t::state_t::Chase;
        for (auto g : state.ghosts)
          g->changeState(s);
      }
      else {
        s = ghost_t::state_t::Runaway;
        for (auto g : state.ghosts)
          g->changeState(s);
      }
    }
  }

  t2 = t1;
}