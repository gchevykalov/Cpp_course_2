#pragma once
#include <vector>
#include <queue>
#include <string>
#include <functional>

class Menu_t;
class MsgBox_t;

class MenuSystem_t {
private:
  Menu_t *mainMenu;
  std::queue<MsgBox_t *> msgBoxes;
public:
  MenuSystem_t(void);

  void buildMenu(std::string fName);
  void addFunction(std::string name, std::function<void(std::vector<std::string>)> func);
  void draw(void);
  void clickHandler(unsigned key);
  void resize(int W, int H);
  void addMsgBox(std::string title, std::string Msg);

  ~MenuSystem_t();
};