#include "LoginWindow.hpp"
#include "anim.hpp"
#include <QApplication>
#include <QObject>
#include <QStackedWidget>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QStackedWidget stacked;
  LoginWindow *login = new LoginWindow;
  MainPage *mainp = new MainPage;

  stacked.addWidget(login);
  stacked.addWidget(mainp);

  QObject::connect(login, &LoginWindow::loginSucceeded, &stacked,
                   [&stacked, mainp]() {
                     stacked.setCurrentWidget(mainp);
                     fadeInWidget(mainp, 400);
                   });
  QObject::connect(mainp, &MainPage::requestLogout, &stacked,
                   [&stacked, login]() {
                     stacked.setCurrentWidget(login);
                     fadeInWidget(login, 300);
                   });
  stacked.resize(1300, 660);
  stacked.show();
  return app.exec();
}
