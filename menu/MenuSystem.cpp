#include "MenuSystem.h"
#include "MenuElements.h"

MenuSystem_t::MenuSystem_t(void) {
  mainMenu = new Menu_t;
}

void MenuSystem_t::buildMenu(std::string fName) {
  mainMenu->buildMenu(fName);
}

void MenuSystem_t::addFunction(std::string name, std::function<void(std::vector<std::string>)> func) {
  Button_t::addFunction(name, func);
}

void MenuSystem_t::draw() {
  if (!msgBoxes.empty())
    msgBoxes.front()->draw(true);
  else {
    if(Menu_t::activeEmpty())
      mainMenu->drawMenu();
    else
      Menu_t::topActive()->drawMenu();
  }
}

void MenuSystem_t::clickHandler(unsigned key) {
  if (!msgBoxes.empty()) {
    msgBoxes.front()->clickHandler(key);
    if (msgBoxes.front()->isClose) {
      MsgBox_t* el = msgBoxes.front();
      msgBoxes.pop();
      delete el;
    }
  }
  else {
    if (Menu_t::activeEmpty())
      mainMenu->clickHandler(key);
    else
      Menu_t::topActive()->clickHandler(key);
  }
}

void MenuSystem_t::resize(int W, int H) {
  MenuItem_t::resize(W, H);
}

void MenuSystem_t::addMsgBox(std::string title, std::string Msg) {
  msgBoxes.push(new MsgBox_t(title, Msg));
}

MenuSystem_t::~MenuSystem_t() {
  delete mainMenu;
}