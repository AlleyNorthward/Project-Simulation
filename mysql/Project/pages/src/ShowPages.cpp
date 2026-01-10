#include "ShowPages.hpp"
#include "anim.hpp"
#include "path.hpp"
#include <QIcon>

ShowPages::ShowPages(QWidget *parent) {
  setupMainPages();

  login = new LoginWindow;
  mainp = new MainPage;

  this->addWidget(login);
  this->addWidget(mainp);

  connect(login, &LoginWindow::loginSucceeded, this, &ShowPages::setLoginPage);
  connect(mainp, &MainPage::requestLogout, this, &ShowPages::setMainPage);
}

void ShowPages::setupMainPages() {
  setWindowTitle("山东科技酒店");
  QString iconPath = utils::get_file_path("assets/icons/04hotel.svg");
  setWindowIcon(QIcon(iconPath));
  resize(1300, 660);
}

void ShowPages::setLoginPage() {
  this->setCurrentWidget(mainp);
  fadeInWidget(mainp, 400);
}

void ShowPages::setMainPage() {
  this->setCurrentWidget(login);
  fadeInWidget(login, 300);
}
