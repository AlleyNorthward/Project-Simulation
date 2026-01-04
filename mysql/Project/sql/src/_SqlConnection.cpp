#include <_SqlConnection.hpp>
#include <QString>
#include <QUuid>
#include <QSqlError>

_SqlConnection::_SqlConnection(){
  this->connectionName = QString("_SqlConnection_%1").arg(QUuid::createUuid().toString());
}

_SqlConnection& _SqlConnection::instance(){
  static _SqlConnection inst;
  return inst;
}

bool _SqlConnection::open(
    const QString& driver, const QString& databasename,
    const QString& user, const QString& password
    ){

  if(QSqlDatabase::contains(this->connectionName)){
    QSqlDatabase db = QSqlDatabase::database(this->connectionName);
    if(db.isOpen()){
      return true;
    }
  }

  QSqlDatabase db = QSqlDatabase::addDatabase(driver, this->connectionName);
  db.setDatabaseName(databasename);
  db.setUserName(user);
  db.setPassword(password);
  if(!db.open()){
    qWarning() << "failed:" << db.lastError().text();
    return false;
  }
  return true;
}

QSqlDatabase _SqlConnection::database() const{
  if(QSqlDatabase::contains(this->connectionName)){
    return QSqlDatabase::database(this->connectionName);
  }
  return QSqlDatabase();
}

void _SqlConnection::close(){
  if(QSqlDatabase::contains(this->connectionName)){
    {
      QSqlDatabase db = QSqlDatabase::database(this->connectionName);
      if(db.isOpen())
        db.close();
      
    }
    QSqlDatabase::removeDatabase(this->connectionName);
  }
}










