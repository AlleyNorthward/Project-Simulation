#include "SqlConnection.hpp"
#include <QCoreApplication>
#include <QSqlQuery>
#include <QTextStream>
#include <cstdio>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  QTextStream out(stdout);
  out.setEncoding(QStringConverter::Utf8);
  if (!SqlConnection::instance().open("QODBC", "mysql", "root",
                                      "2417A534000")) {
    out << "数据库打开失败, 程序退出" << "\n";
    return -1;
  }
  out << "数据库连接成功" << "\n";

  QSqlQuery query(SqlConnection::instance().database());
  query.exec("SELECT roomTypeId, name, basePrice FROM RoomType");

  query.next();
  int id = query.value("roomTypeId").toInt();
  out << id << "\n";
  query.exec("SHOW TABLES");
  query.next();
  QString tableName = query.value(0).toString();
  out << tableName << "\n";
  return 0;
  // return a.exec();
}
