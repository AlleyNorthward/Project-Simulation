#include "OrderTab.hpp"
#include "SqlQuery.hpp"
#include <QHeaderView>
#include <QLabel>
#include <QVBoxLayout>

OrderTab::OrderTab(QWidget *parent) : QWidget(parent) {
  auto *v = new QVBoxLayout(this);
  auto *h = new QHBoxLayout;

  m_table = new QTableView(this);
  m_model = new QStandardItemModel(0, 5, this);
  m_model->setHeaderData(0, Qt::Horizontal, "手机号");
  m_model->setHeaderData(1, Qt::Horizontal, "入住时间");
  m_model->setHeaderData(2, Qt::Horizontal, "退房时间");
  m_model->setHeaderData(3, Qt::Horizontal, "状态");
  m_model->setHeaderData(4, Qt::Horizontal, "金额");
  getDatabaseInfos();
  m_table->setModel(m_model);
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_table->verticalHeader()->setVisible(false);

  h->addWidget(m_table);
  v->addLayout(h);
}

void OrderTab::addRow(const QString &r, const QString &t, const QString &p,
                      const QString &s, const QString &note) {

  QList<QStandardItem *> row;
  row << new QStandardItem(r) << new QStandardItem(t) << new QStandardItem(p)
      << new QStandardItem(s) << new QStandardItem(note);
  for (auto *item : row) {
    item->setTextAlignment(Qt::AlignCenter);
    item->setEditable(false);
  }
  m_model->appendRow(row);
}

void OrderTab::getDatabaseInfos() {
  QSqlQuery &Query = SqlQuery::instance().query();
  QString queryStr = R"(
    SELECT
      c.phone,
      r.checkInDate,
      r.checkOutDate,
      r.status,
      r.depositAmount
    FROM Reservation r
    JOIN Customer c
      ON r.customerId = c.customerId
    ORDER BY r.checkInDate ASC;
  )";

  if (Query.exec(queryStr)) {
    while (Query.next()) {
      addRow(Query.value(0).toString(), Query.value(1).toString(),
             Query.value(2).toString(), Query.value(3).toString(),
             Query.value(4).toString());
    }
  }
}
