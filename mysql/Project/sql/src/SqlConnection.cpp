#include "SqlConnection.hpp"
#include "out.hpp"
#include <QDebug>
#include <QSqlError>
#include <QUuid>

SqlConnection &SqlConnection::instance() {
  static SqlConnection inst;
  return inst;
}

SqlConnection::SqlConnection() {
  this->cononectionName =
      QString("SqlConnection_%1").arg(QUuid::createUuid().toString());
}

// SqlConnection::~SqlConnection() { this->close(); }

bool SqlConnection::open(const QString &database, const QString &databasename,
                         const QString &user, const QString &password) {
  if (QSqlDatabase::contains(this->cononectionName)) {
    QSqlDatabase db = QSqlDatabase::database(this->cononectionName);
    if (db.isOpen())
      return true;
  }

  QSqlDatabase db = QSqlDatabase::addDatabase(database, this->cononectionName);
  db.setDatabaseName(databasename);
  db.setUserName(user);
  db.setPassword(password);
  if (!db.open()) {
    utils::out() << "连接失败" << db.lastError().text();
    return false;

  }
  utils::out() << "数据库连接成功!" << utils::endl;
  return true;
}

QSqlDatabase SqlConnection::database() const {
  if (QSqlDatabase::contains(this->cononectionName))
    return QSqlDatabase::database(this->cononectionName);
  return QSqlDatabase();
}

void SqlConnection::close() {
  if (QSqlDatabase::contains(this->cononectionName)) {
    {
      QSqlDatabase db = QSqlDatabase::database(this->cononectionName);
      if (db.isOpen())
        db.close();
    }
    QSqlDatabase::removeDatabase(this->cononectionName);
  }
}
