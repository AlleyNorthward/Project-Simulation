#ifndef SQLCONNECTION_HPP
#define SQLCONNECTION_HPP
#include <QSqlDatabase>
#include <QString>

class SqlConnection {
public:
  static SqlConnection &instance();
  bool open(const QString &database, const QString &databasename,
            const QString &user, const QString &password);
  QSqlDatabase database() const;
  void close();

private:
  QString cononectionName;
  SqlConnection();

  // 这里的析构函数, 视情况而定吧. 目前就用默认的析构函数就好.
  ~SqlConnection() = default;
  SqlConnection(const SqlConnection &) = delete;
  SqlConnection &operator=(const SqlConnection &) = delete;
};

#endif
