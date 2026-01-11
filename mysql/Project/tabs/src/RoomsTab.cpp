#include "RoomsTab.hpp"
#include "SqlQuery.hpp"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

_RoomSearch::_RoomSearch(QWidget *parent) : QWidget(parent) {
  auto *h = new QHBoxLayout(this);
  h->setSpacing(0);
  h->setContentsMargins(0, 0, 0, 0);

  h->addWidget(new QLabel("搜索:", this));
  m_search = new QLineEdit(this);
  m_search->setPlaceholderText("按房号/类型/状态/价格/备注搜索");
  h->addWidget(m_search);

  h->addWidget(new QLabel("状态:", this));
  m_filter = new QComboBox(this);
  m_filter->addItems({"全部", "空闲", "已入住", "维修中"});
  h->addWidget(m_filter);

  m_refresh = new QPushButton("刷新", this);
  h->addWidget(m_refresh);
}

_RoomTable::_RoomTable(QWidget *parent) : QWidget(parent) {
  auto *h = new QHBoxLayout(this);
  m_table = new QTableView(this);
  m_model = new QStandardItemModel(0, 5, this);
  m_model->setHeaderData(0, Qt::Horizontal, "房号");
  m_model->setHeaderData(1, Qt::Horizontal, "类型");
  m_model->setHeaderData(2, Qt::Horizontal, "价格");
  m_model->setHeaderData(3, Qt::Horizontal, "状态");
  m_model->setHeaderData(4, Qt::Horizontal, "备注");

  getDatabaseInfos();

  m_proxy = new QSortFilterProxyModel(this);
  m_proxy->setSourceModel(m_model);
  m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

  m_proxy->setFilterKeyColumn(-1);

  m_table->setModel(m_proxy);
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_table->verticalHeader()->setVisible(false);
  h->addWidget(m_table);
}

void _RoomTable::addRow(const QString &r, const QString &t, const QString &p,
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

RoomsTab::RoomsTab(QWidget *parent) : QWidget(parent) {
  auto *v = new QVBoxLayout(this);
  roomSearch = new _RoomSearch(this);
  roomTable = new _RoomTable(this);
  v->addWidget(roomSearch);
  v->addWidget(roomTable);

  connect(roomSearch->getSearch(), &QLineEdit::textChanged, this,
          &RoomsTab::onSearch);
  connect(roomSearch->getFilter(),
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &RoomsTab::onFilter);
  connect(roomSearch->getRefresh(), &QPushButton::clicked, this,
          &RoomsTab::onRefresh);
}

void RoomsTab::onSearch(const QString &text) {
  roomTable->getProxy().setFilterKeyColumn(-1);
  QRegularExpression re(text, QRegularExpression::CaseInsensitiveOption);
  roomTable->getProxy().setFilterRegularExpression(re);
}

void RoomsTab::onFilter(int idx) {
  QString status = roomSearch->getFilter()->currentText();
  if (status == "全部") {
    roomTable->getProxy().setFilterFixedString(QString());
    roomTable->getProxy().setFilterKeyColumn(-1);
  } else {
    roomTable->getProxy().setFilterKeyColumn(3);
    roomTable->getProxy().setFilterFixedString(status);
  }
}

void RoomsTab::onRefresh() {
  roomTable->getModel().removeRows(0, roomTable->getModel().rowCount());
  roomTable->getDatabaseInfos();
}

void _RoomTable::getDatabaseInfos() {
  QSqlQuery &Query = SqlQuery::instance().query();
  QString queryStr = R"(
    SELECT
      r.roomId,
      rt.name,
      rt.basePrice,
      r.status,
      r.description
    FROM Room r
    JOIN RoomType rt
      ON r.roomTypeId = rt.roomTypeId
    ORDER BY r.roomId ASC;
  )";

  if (Query.exec(queryStr)) {
    while (Query.next()) {
      addRow(Query.value(0).toString(), Query.value(1).toString(),
             Query.value(2).toString(), Query.value(3).toString(),
             Query.value(4).toString());
    }
  }
}
