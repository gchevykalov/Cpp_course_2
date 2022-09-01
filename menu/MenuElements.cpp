#include <iterator>
#include <sstream>
#include <fstream>
#include <iostream>
#include "MenuElements.h"
#include "GL/freeglut.h"

mmap Button_t::functionMap;
int MenuItem_t::elementW;
int MenuItem_t::windowH, MenuItem_t::windowW;
std::stack<Menu_t*> Menu_t::activeSubmenus;
constexpr unsigned FONTHEIGHT = 24;
constexpr unsigned BOUNDARES = 3;

static void alignment(const std::string& line, unsigned int width, std::vector<std::string> *lines) {
  std::string str;

  lines->clear();
  if (glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)line.c_str()) <= width)
    lines->push_back(line);//suitable length
  else {
    unsigned int length = line.length(), pos = 0;
    //too long word
    while (length > 0) {
      str = "";
      do {
        str += line.substr(pos, 1);
        length--;
        pos++;
      } while (length > 0 && (glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)str.c_str()) 
        + glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, line[pos])) <= width);
      lines->push_back(str);
    }
  }
}

static void back(std::vector<std::string> el) {
  Menu_t::activePop();
}

/* MenuItem_t func */
void MenuItem_t::setElemWidth(int W) {
  if (W < 100) {
    std::cerr << "Too small width, so the default width is used" << std::endl;
    elementW = 100;
  }
  else
    elementW = W;
}

void MenuItem_t::resize(int W, int H) {
  windowW = W;
  windowH = H;
}

void MenuItem_t::setUpperCorner(int x, int y) {
  elemX = x;
  elemY = y;
}

/* GLUT drawing func */
void drawRect(int x, int y, int w, int h) {
  double x1 = x + w;
  double y1 = y - h;
  glRectd(x, y, x1, y1);
}

void drawString(int x, int y, int w, void* font, std::vector<std::string>* lines) {
  double y1 = y - BOUNDARES - FONTHEIGHT;
  for (auto& l : *lines) {
    double x1 = (w - glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)l.c_str())) / 2;
    glRasterPos2d(x + x1, y1);
    glutBitmapString(font, (const unsigned char*)l.c_str());
    y1 -= FONTHEIGHT + BOUNDARES;
  }
}

/* Button_t func */
void Button_t::addFunction(std::string name, std::function<void(std::vector<std::string>)> func) {
  functionMap[name] = func;
}

int Button_t::draw(bool isActive) {
  std::vector<std::string> lines;
  alignment(name, elementW - 2 * BOUNDARES, &lines);
  if (isActive)
    glColor3d(0.0, 0.3, 0.5);
  else
    glColor3d(0.0, 0.4, 0.6);
  drawRect(elemX, elemY, elementW, (FONTHEIGHT + BOUNDARES) * lines.size() + 2 * BOUNDARES);
  glColor3d(0, 0, 0);
  drawString(elemX, elemY, elementW, GLUT_BITMAP_TIMES_ROMAN_24, &lines);

  return (FONTHEIGHT + BOUNDARES) * lines.size() + 2 * BOUNDARES;
}

void Button_t::clickHandler(unsigned char key) {
  if (functionMap.count(functionName))
    functionMap[functionName](args);
  else
    std::cerr << "A non-existing command assigned to this button" << std::endl;
}

/* Menu_t func */
Menu_t* Menu_t::topActive(void) {
  return activeSubmenus.top();
}

bool Menu_t::activeEmpty(void) {
  return activeSubmenus.empty();
}

void Menu_t::activePop(void) {
  activeSubmenus.pop();
}

void Menu_t::activePush(Menu_t* M) {
  activeSubmenus.push(M);
}

void Menu_t::drawMenu(void) {
  elemX = (windowW - elementW) / 2;
  elemY = windowH - 30;
  std::vector<std::string> lines;
  int prevH = elemY, delta;
  alignment(menuName, elementW - 2 * BOUNDARES, &lines);
  glColor3d(1, 1, 1);
  drawString(elemX, elemY, elementW, GLUT_BITMAP_TIMES_ROMAN_24, &lines);
  delta = (FONTHEIGHT + BOUNDARES) * lines.size() + 42;

  for (size_t i = 0; i < items.size(); i++) {
    prevH -= delta;
    items[i]->setUpperCorner(elemX, prevH);
    delta = items[i]->draw(i == chosen);
  }
}

int Menu_t::draw(bool isActive) {
  std::vector<std::string> lines;
  alignment(menuName + " >", elementW - 2 * BOUNDARES, &lines);
  if (isActive)
    glColor3d(0.0, 0.3, 0.5);
  else
    glColor3d(0.0, 0.4, 0.6);
  drawRect(elemX, elemY, elementW, (FONTHEIGHT + BOUNDARES) * lines.size() + 2 * BOUNDARES);
  glColor3d(0, 0, 0);
  drawString(elemX, elemY, elementW, GLUT_BITMAP_TIMES_ROMAN_24, &lines);

  return (FONTHEIGHT + BOUNDARES) * lines.size() + 2 * BOUNDARES;
}

void Menu_t::clickHandler(unsigned char key) {
    if (key == 'W' || key == 'w' )
      chosen = (chosen - 1 + items.size()) % items.size();
    if (key == 'S' || key == 's' )
      chosen = (chosen + 1 + items.size()) % items.size();
    if (key == ' ' || key == 13) {
      if (items[chosen]->type == MenuItem_t::ButtonType)
        items[chosen]->clickHandler(key);
      if (items[chosen]->type == MenuItem_t::MenuType)
        activePush((Menu_t *)items[chosen]);
    }
}

void Menu_t::buildMenu(std::string filename) {
  Button_t::addFunction("back", back);
  std::fstream file(filename);
  std::string buf;
  std::stack<Menu_t*> menus;
  menus.push(this);
  while (std::getline(file, buf)) {
    std::stringstream S(buf);
    std::istream_iterator<std::string> iter(S);
    std::string keyword = *iter;
    iter++;
    if (keyword == "Width") {
      setElemWidth(stod(*iter));
    }
    else if (keyword == "MainMenu") {
      menus.top()->menuName = *iter;
    }
    else if (keyword == "Button") {
      std::string name = *iter;
      iter++;
      std::string func_name = *iter;
      iter++;
      std::vector<std::string> args;
      std::copy(iter, std::istream_iterator<std::string>(), std::inserter(args, args.end()));
      menus.top()->items.push_back(new Button_t(name, func_name, args));
    }
    else if (keyword == "SubMenu") {
      Menu_t *M = new Menu_t;
      M->menuName = *iter;
      menus.top()->items.push_back(M);
      menus.push(M);
    }
    else if (keyword == "EndSubMenu") {
      if (menus.empty())
        throw "Incorrect Menu file";
      std::string Subfilename = *iter;
      std::vector<std::string> args;
      menus.top()->items.push_back(new Button_t("< back", "back", args));
      menus.pop();
    }
    else {
      throw "Incorrect Menu file";
    }
  }
}

Menu_t::~Menu_t(void) {
  for (auto i : items)
    delete i;
}

/* Message Box func */
int MsgBox_t::draw(bool isActive) {
  int heigth = 0, delta;
  std::vector<std::string> lines;
  alignment(title, elementW - 2 * BOUNDARES, &lines);
  elemX = (windowW - elementW) / 2;
  elemY = windowH - windowH / 6;
  glColor3d(0.0, 0.6, 0.6);
  drawRect(elemX, elemY, elementW, (FONTHEIGHT + BOUNDARES) * lines.size() + 2 * BOUNDARES);
  heigth += (FONTHEIGHT + BOUNDARES) * lines.size() + 2 * BOUNDARES;
  glColor3d(0, 0, 0);
  drawString(elemX, elemY, elementW, GLUT_BITMAP_TIMES_ROMAN_24, &lines);

  delta = (FONTHEIGHT + BOUNDARES) * lines.size() + 2 * BOUNDARES;

  alignment(message, elementW - 2 * BOUNDARES, &lines);
  glColor3d(0.5, 0.5, 0.5);
  drawRect(elemX, elemY - delta, elementW, (FONTHEIGHT + BOUNDARES) * lines.size() + 2 * BOUNDARES);
  heigth += (FONTHEIGHT + BOUNDARES) * lines.size() + 2 * BOUNDARES;
  glColor3d(0, 0, 0);
  drawString(elemX, elemY - delta, elementW, GLUT_BITMAP_TIMES_ROMAN_24, &lines);

  return heigth;
}

void MsgBox_t::clickHandler(unsigned char key) {
  isClose = true;
}
