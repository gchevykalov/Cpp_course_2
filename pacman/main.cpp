#include "Game.h"

void displayFunc(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  game_t::getInst()->move();
  game_t::getInst()->draw();
  
  glutSwapBuffers();
  Sleep(150);
  glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y) {
  game_t::getInst()->clickHandler(key);
}

void resize(int w, int h) {
  glViewport(0, 0, w, h);
  game_t::getInst()->reshape(w, h);
}

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 480);
  game_t::getInst()->reshape(640, 480);
  glutCreateWindow("pacman");

  game_t::getInst()->loadMap("map.txt");

  glutDisplayFunc(displayFunc);
  glutKeyboardFunc(keyboardFunc);
  glutReshapeFunc(resize);

  glutMainLoop();

  game_t::getInst()->end();

  return 0;
}