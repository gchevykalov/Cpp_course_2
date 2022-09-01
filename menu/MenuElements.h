#pragma once
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <functional>

using mmap = std::map<std::string, std::function<void(std::vector<std::string>)>>;

class MenuItem_t {
protected:
  static int elementW;
  static int windowH;
  static int windowW;
  int elemX, elemY;
public:
  enum ItemType {
    MenuType,
    ButtonType,
    MessageBoxType
  };
  ItemType type;
  MenuItem_t(ItemType T) : type(T), elemX(100), elemY(100) {}

  static void setElemWidth(int W);
  static void resize(int W, int H);
  void setUpperCorner(int x, int y);

  virtual int draw(bool isActive) = 0;
  virtual void clickHandler(unsigned char key) = 0;
};

class Button_t : public MenuItem_t {
private:
  static mmap functionMap;
  std::string name;
  std::string functionName;
  std::vector<std::string> args;
public:
  Button_t(std::string BName, std::string funcName, std::vector<std::string>& BArgs) : MenuItem_t(MenuItem_t::ButtonType),
    name(BName), functionName(funcName), args(BArgs) {}
  static void addFunction(std::string name, std::function<void(std::vector<std::string>)> func);

  virtual int draw(bool isActive) override;
  virtual void clickHandler(unsigned char key) override;
};

class Menu_t : public MenuItem_t {
private:
  std::vector<MenuItem_t*> items;
  int chosen;
  std::string menuName;
  static std::stack<Menu_t*> activeSubmenus;
public:
  Menu_t(void) : MenuItem_t(MenuItem_t::MenuType), chosen(0) {}
  void buildMenu(std::string filename);
  void drawMenu(void);

  static Menu_t* topActive(void);
  static bool activeEmpty(void);
  static void activePop(void);
  static void activePush(Menu_t* M);

  virtual int draw(bool isActive) override;
  virtual void clickHandler(unsigned char key) override;
  ~Menu_t(void);
};

class MsgBox_t : public MenuItem_t {
private:
  std::string title;
  std::string message;
public:
  bool isClose;
  MsgBox_t(std::string Mtitle, std::string Msg) : MenuItem_t(MenuItem_t::MessageBoxType), title(Mtitle), message(Msg), isClose(false) {};

  virtual int draw(bool isActive) override;
  virtual void clickHandler(unsigned char key) override;
};