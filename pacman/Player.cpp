#include "Player.h"
#include "StaticGameObjects.h"

const std::map<unit_t::direction_t, geom_vector_t> unit_t::dirVecs = {
  {unit_t::direction_t::Up, geom_vector_t(0, -1)},
  {unit_t::direction_t::Down, geom_vector_t(0, 1)},
  {unit_t::direction_t::Right, geom_vector_t(1, 0)},
  {unit_t::direction_t::Left, geom_vector_t(-1, 0)}
};

void player_t::move(void) {
  geom_vector_t v, tmp;

  v = dirVecs.at(curDirection);

  tmp = v + pos;
  if (tmp.x >= gameState->mapW || tmp.x < 0)
    tmp.x = (gameState->mapW + tmp.x) % gameState->mapW;
  if (tmp.y >= gameState->mapH || tmp.y < 0)
    tmp.y = (gameState->mapH + tmp.y) % gameState->mapH;

  if (gameState->map[tmp.y][tmp.x]->allowMove()) {
    gameState->map[tmp.y][tmp.x]->eaten();
    pos = tmp;
  }
}

void player_t::changeDirection(int key) {
  switch (key) {
  case 'w':
    curDirection = unit_t::direction_t::Up;
    break;
  case 's':
    curDirection = unit_t::direction_t::Down;
    break;
  case 'a':
    curDirection = unit_t::direction_t::Left;
    break;
  case 'd':
    curDirection = unit_t::direction_t::Right;
    break;
  default:
    break;
  }
}

void player_t::draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) {
  int x1 = upperLeftCorner.x + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW * pos.x +
    (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW / 2;
  int y1 = upperLeftCorner.y + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH * (gameState->mapH - 1 - pos.y)
    + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH / 2;
  int r;

  if ((lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW <
    (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH)
    r = (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW / 2;
  else
    r = (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH / 2;

  glColor3d(1, 179. / 255, 0);
  double twicePi = 2.0 * 3.142;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2i(x1, y1);
  for (int i = 0; i <= 20; i++) {
    glVertex2i(
      (x1 + (r * cos(i * twicePi / 20))), (y1 + (r * sin(i * twicePi / 20)))
    );
  }
  glEnd();
}

void player_t::respawn(void) {
  gameState->isplayerAlive = true;
  pos = startPos;
  curDirection = unit_t::direction_t::Up;
  gameState->fruitCounter -= gameState->curLevFruitCount;
  gameState->curLevFruitCount = 0;
}