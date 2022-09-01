#include "StaticGameObjects.h"
#include "Ghost.h"

const second_t booster_t::duration = second_t(10);
const second_t fruit_t::lifetime = second_t(9);

void wall_t::draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) {
  int x1 = upperLeftCorner.x + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW * pos.x;
  int x2 = upperLeftCorner.x + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW * (pos.x + 1) - 1;
  int y1 = upperLeftCorner.y + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH * (gameState->mapH - 1 - pos.y);
  int y2 = upperLeftCorner.y + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH * (gameState->mapH - pos.y) - 1;

  glColor3d(205. / 255, 133. / 255, 63. / 255);
  glRecti(x1, y1, x2, y2);
}

bool wall_t::allowMove(void) {
  return false;
}

void wall_t::eaten(void) {
  return;
}

void ghost_door_t::draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) {
  int x1 = upperLeftCorner.x + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW * pos.x;
  int x2 = upperLeftCorner.x + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW * (pos.x + 1) - 1;
  int y1 = upperLeftCorner.y + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH * (gameState->mapH - 1 - pos.y) + 
    (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH / 2;
  int y2 = upperLeftCorner.y + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH * (gameState->mapH - pos.y) - 1;

  glColor3d(205. / 255, 133. / 255, 63. / 255);
  glRecti(x1, y1, x2, y2);
}

bool ghost_door_t::allowMove(void) {
  return false;
}

void ghost_door_t::eaten(void) {
  return;
}

void empty_t::draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) {
  return;
}

bool empty_t::allowMove(void) {
  return true;
}

void empty_t::eaten(void) {
  return;
}

void food_t::draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) {
  if (!isActive)
    return;

  int x1 = upperLeftCorner.x + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW * pos.x +
    (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW / 2;
  int y1 = upperLeftCorner.y + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH * (gameState->mapH - 1 - pos.y) +
    (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH / 2;

  glColor3d(205. / 255, 219. / 255, 139. / 255);
  glRecti(x1 - 2, y1 - 2, x1 + 2, y1 + 2);
}

bool food_t::allowMove(void) {
  return true;
}

void food_t::eaten(void) {
  if (!isActive)
    return;
  
  gameState->foodCounter++;
  gameState->playerScore += points * gameState->level;
  isActive = false;
}

void booster_t::draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) {
  if (!isActive)
    return;

  int x1 = upperLeftCorner.x + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW * pos.x +
    (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW / 2;
  int y1 = upperLeftCorner.y + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH * (gameState->mapH - 1 - pos.y) +
    (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH / 2;

  glColor3d(205. / 255, 219. / 255, 139. / 255);
  glRecti(x1 - (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW / 4,
    y1 - (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH / 4,
    x1 + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW / 4,
    y1 + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH / 4);
}

bool booster_t::allowMove(void) {
  return true;
}

void booster_t::eaten(void) {
  if (!isActive)
    return;

  for (auto g : gameState->ghosts)
    g->changeState(ghost_t::state_t::Panic);
  gameState->duration = duration;
  gameState->playerScore += points * gameState->level;
  gameState->factor = 1;
  isActive = false;
}

void fruit_t::draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) {
  if (!isActive)
    return;

  int x1 = upperLeftCorner.x + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW * pos.x +
    (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW / 2;
  int y1 = upperLeftCorner.y + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH * (gameState->mapH - 1 - pos.y) +
    (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH / 2;
  
  glColor3d(0, 1, 0);
  glRecti(x1 - (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW / 4,
    y1 - (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH / 4,
    x1 + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW / 4,
    y1 + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH / 4);
}

bool fruit_t::allowMove(void) {
  return true;
}

void fruit_t::eaten(void) {
  if (!isActive)
    return;

  gameState->curLevFruitCount++;
  gameState->fruitCounter++;
  gameState->playerScore += points * gameState->level;
  isActive = false;
}

void fruit_t::set(void) {
  isActive = true;
  gameState->lifetime = lifetime;
}

void fruit_t::remove(void) {
  isActive = false;
}