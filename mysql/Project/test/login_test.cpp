#include "LoginWindow.hpp"
#include <QApplication>
#include <QObject>
#include "out.hpp"
#include "SqlConnection.hpp"
#include <QStackedWidget>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QStackedWidget stacked;
  if (!SqlConnection::instance().open("QODBC", "mysql", "root",
                                      "2417A534000")) {
    utils::out() << "数据库打开失败, 程序退出" << utils::endl;
    return -1;
  }
  LoginWindow *login = new LoginWindow;

  stacked.addWidget(login);

  stacked.resize(1300, 660);
  stacked.show();
  return app.exec();
}
