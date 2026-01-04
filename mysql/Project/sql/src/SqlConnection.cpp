#include "SqlConnection.hpp"
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
    qWarning() << "failed:" << db.lastError().text();
    return false;
  }
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
