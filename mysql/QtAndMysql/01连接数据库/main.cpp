#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>

#include <QDebug>

int main(int argc, char *argv[]){
  QCoreApplication a(argc, argv);

  qDebug() << "Qt drivers:" << QSqlDatabase::drivers();
  QSqlDatabase database = QSqlDatabase::addDatabase("QODBC");
  database.setDatabaseName("mysql");
  database.setUserName("root");
  database.setPassword("2417A534000");

  if (!database.open()){
    qDebug() << "failed:" << database.lastError().text();
    return -1;
  }

  qDebug() << "success!" << "\n";
}
