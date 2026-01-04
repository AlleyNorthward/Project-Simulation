#include <QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>
#include <cstdio>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  QTextStream out(stdout);
  out.setEncoding(QStringConverter::Utf8);

  // out << "Qt drivers:" << QSqlDatabase::drivers();
  qDebug() << "Qt drivers:" << QSqlDatabase::drivers();

  QSqlDatabase database = QSqlDatabase::addDatabase("QODBC");
  database.setDatabaseName("mysql");
  database.setUserName("root");
  database.setPassword("2417A534000");

  if (!database.open()) {
    out << "连接失败:" << database.lastError().text();
  }

  out << "连接成功" << "\n";

  QSqlQuery query;

  if (!query.exec("SELECT roomTypeId, name, basePrice FROM RoomType")) {
    out << "失败:" << query.lastError().text();
    return 0;
  }

  while (query.next()) {
    int id = query.value("roomTypeId").toInt();
    QString name = query.value("name").toString();
    double price = query.value("basePrice").toDouble();

    out << id << "\n" << name << "\n" << price << "\n";
  }

  return 0;
}
