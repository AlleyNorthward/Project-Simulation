#include "LoginWindow.hpp"
#include <QApplication>
#include <QObject>
#include <QStackedWidget>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QStackedWidget stacked;
  LoginWindow *login = new LoginWindow;

  stacked.addWidget(login);

  stacked.resize(1300, 660);
  stacked.show();
  return app.exec();
}
