#include "MenuSystem.h"
#include "GL/freeglut.h"

MenuSystem_t gMenu;

void endSession(std::vector<std::string> el) {
  exit(0);
}

void simpleMsgBox(std::vector<std::string> el) {
  gMenu.addMsgBox("simple message box", "some text");
}

void msgBoxWithArgs(std::vector<std::string> el) {
  std::string args;
  for (auto& e : el)
    args += e + "  ";
  gMenu.addMsgBox("message box", "args: " + args);
}

void displayFunc(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double w = glutGet(GLUT_WINDOW_WIDTH);
  double h = glutGet(GLUT_WINDOW_HEIGHT);
  glOrtho(0, w, 0, h, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gMenu.draw();
  glutSwapBuffers();
  glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y) {
  gMenu.clickHandler(key);
}

void resize(int w, int h) {
  glViewport(0, 0, w, h);
  gMenu.resize(w, h);
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 480);
  gMenu.resize(640, 480);
  glutCreateWindow("lab3 - Menu system");

  gMenu.addFunction("exit", endSession);
  gMenu.addFunction("simpleMB", simpleMsgBox);
  gMenu.addFunction("MB", msgBoxWithArgs);

  gMenu.buildMenu("MainMenu.msf");

  glutDisplayFunc(displayFunc);
  glutKeyboardFunc(keyboardFunc);
  glutReshapeFunc(resize);

  glutMainLoop();

  return 0;
}