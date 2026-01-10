#include "SqlConnection.hpp"
#include "SqlQuery.hpp"
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

  SqlQuery &sqlQuery = SqlQuery::instance();
  QSqlQuery& Query = sqlQuery.query();

  QString queryStr =
      "SELECT username, password FROM Employee WHERE username = ? "
      "AND password = ?";

  Query.prepare(queryStr);
  Query.bindValue(0, "wangqianlong");
  Query.bindValue(1, "123456");

  if (Query.exec()) {
    Query.bindValue(0, "wangqianlong");
    Query.bindValue(1, "123456");
    utils::out() << Query.boundValue(0).toString() << " "
                 << Query.boundValue(1).toString() << utils::endl;

    if (Query.next()) {
      utils::out() << "success" << utils::endl;
    }
  }
  return 0;
}
