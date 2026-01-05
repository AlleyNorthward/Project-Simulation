#include "SqlConnection.hpp"
#include "out.hpp"
#include <QCoreApplication>
#include <QSqlQuery>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  if (!SqlConnection::instance().open("QODBC", "mysql", "root",
                                      "2417A534000")) {
    utils::out() << "数据库打开失败, 程序退出" << utils::endl;
    return -1;
  }
  utils::out() << "诶嘿!" << utils::endl;

  QSqlQuery query(SqlConnection::instance().database());
  query.exec("SELECT roomTypeId, name, basePrice FROM RoomType");

  query.next();
  int id = query.value("roomTypeId").toInt();
  utils::out() << id << utils::endl;
  query.exec("SHOW TABLES");
  query.next();
  QString tableName = query.value(0).toString();
  utils::out() << tableName << utils::endl;
  return 0;
}
