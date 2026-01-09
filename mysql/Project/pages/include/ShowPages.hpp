#ifndef SHOW_PAGES_HPP
#define SHOW_PAGES_HPP

#include "LoginWindow.hpp"
#include "MainPage.hpp"
#include <QStackedWidget>

class ShowPages : public QStackedWidget {
  Q_OBJECT
public:
  explicit ShowPages(QWidget *parent = nullptr);

private slots:
  void setupMainPages();
  void setLoginPage();
  void setMainPage();

private:
  LoginWindow *login;
  MainPage *mainp;

// private:
//   void setupPages();
};

#endif
