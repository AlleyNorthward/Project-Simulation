#ifndef SQLQUERY_HPP
#define SQLQUERY_HPP

#include <QSqlQuery>
#include <QString>

class SqlQuery {
public:
  static SqlQuery &instance();
  QSqlQuery &query();

private:
  SqlQuery();
  SqlQuery(const SqlQuery &) = delete;
  SqlQuery &operator=(const SqlQuery &) = delete;

private:
  QSqlQuery m_query;
};
#endif
