#include "ShowPages.hpp"
#include "SqlConnection.hpp"
#include "out.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  if (!SqlConnection::instance().open("QODBC", "mysql", "root",
                                      "2417A534000")) {
    utils::out() << "数据库打开失败, 程序退出" << utils::endl;
    return -1;
  }
  ShowPages pages;
  pages.show();
  return app.exec();
}
