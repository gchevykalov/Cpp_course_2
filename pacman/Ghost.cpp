#include "Ghost.h"
#include "Player.h"
#include "StaticGameObjects.h"

const std::map<unit_t::direction_t, std::vector<unit_t::direction_t>> ghost_t::possibleDirs = {
  {unit_t::direction_t::Up, {unit_t::direction_t::Up, unit_t::direction_t::Right, unit_t::direction_t::Left}},
  {unit_t::direction_t::Down, {unit_t::direction_t::Down, unit_t::direction_t::Right, unit_t::direction_t::Left}},
  {unit_t::direction_t::Right, {unit_t::direction_t::Up, unit_t::direction_t::Down, unit_t::direction_t::Right}},
  {unit_t::direction_t::Left, {unit_t::direction_t::Up, unit_t::direction_t::Down, unit_t::direction_t::Left}}
};
extern std::map<unit_t::direction_t, geom_vector_t> dirVecs;

bool ghost_t::collision(void) {
  if (state == ghost_t::state_t::Panic) {
    gameState->playerScore += points * gameState->factor;
    gameState->factor *= 2;
    this->respawn();
    return false;
  }
  else {
    gameState->playerLives--;
    gameState->isplayerAlive = false;
    return true;
  }
}

bool ghost_t::activate(void) {
  if (isActive)
    return false;

  gameState->activeGhostCount++;
  isActive = true;
  pos = outdoorPos;

  return true;
}

void ghost_t::respawn(void) {
  pos = startPos;
  if (gameState->activeGhostCount > 0)
    gameState->activeGhostCount--;
  curDirection = unit_t::direction_t::Up;
  isActive = false;
  this->changeState(ghost_t::state_t::Chase);
}

void ghost_t::changeState(state_t s) {
  switch (curDirection) {
  case unit_t::direction_t::Up:
    curDirection = unit_t::direction_t::Down;
    break;
  case unit_t::direction_t::Down:
    curDirection = unit_t::direction_t::Up;
    break;
  case unit_t::direction_t::Right:
    curDirection = unit_t::direction_t::Left;
    break;
  default:
    curDirection = unit_t::direction_t::Right;
    break;
  }
  state = s;
}

void ghost_t::draw(geom_vector_t upperLeftCorner, geom_vector_t lowerRightCorner) {
  int x1 = upperLeftCorner.x + (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW * pos.x,
    w = (lowerRightCorner.x - upperLeftCorner.x) / gameState->mapW;
  int y1 = upperLeftCorner.y + (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH * (gameState->mapH - 1 - pos.y),
    h = (lowerRightCorner.y - upperLeftCorner.y) / gameState->mapH;

  if (state == ghost_t::state_t::Panic)
    glColor3d(175. / 255, 238. / 255, 238. / 255);
  else
    glColor3d(r / 255., g / 255., b / 255.);

  glRecti(x1 + w / 3, y1 + h / 10, x1 + w * 2 / 3, y1 + h * 5 / 6);
}

void ghost_t::move(void) {
  geom_vector_t tmp, nextPos;
  std::vector <unit_t::direction_t> v;
  double dist, minDist = 100000;
  unit_t::direction_t nextDir = curDirection;

  if (!isActive) {
    return;
  }

  geom_vector_t target = state == ghost_t::state_t::Chase ? gameState->player->getPos() : checkPoint;
  v = possibleDirs.at(curDirection);
  for (auto d : v) {
    tmp = pos + dirVecs.at(d);
    if (tmp.x >= gameState->mapW || tmp.x < 0)
      tmp.x = (gameState->mapW + tmp.x) % gameState->mapW;
    if (tmp.y >= gameState->mapH || tmp.y < 0)
      tmp.y = (gameState->mapH + tmp.y) % gameState->mapH;
    if (gameState->map[tmp.y][tmp.x]->allowMove()) {
      dist = (tmp - target).Len();
      if (dist < minDist) {
        minDist = dist;
        nextPos = tmp;
        nextDir = d;
      }
    }
  }

  if (nextPos == geom_vector_t(-1, -1))
    return;
  pos = nextPos;
  curDirection = nextDir;
}