#include "SqlQuery.hpp"
#include "SqlConnection.hpp"
#include <QSqlError>

SqlQuery &SqlQuery::instance() {
  static SqlQuery inst;
  return inst;
}

SqlQuery::SqlQuery() {
  m_query = QSqlQuery(SqlConnection::instance().database());
}

QSqlQuery &SqlQuery::query() { return m_query; }
