#ifndef _SQLCONNECTION_HPP
#define _SQLCONNECTION_HPP

#include <QSqlDatabase>
#include <QString>

class _SqlConnection {
private:
  QString connectionName;
  _SqlConnection();
  ~_SqlConnection() = default;
  _SqlConnection(const _SqlConnection &) = delete;
  _SqlConnection &operator=(const _SqlConnection &) = delete;

public:
  static _SqlConnection &instance();
  bool open(const QString &driver, const QString &databasename,
            const QString &user, const QString &password);
  QSqlDatabase database() const;
  void close();
};
#endif
