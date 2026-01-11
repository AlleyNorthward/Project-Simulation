#include "BillTab.hpp"
#include "SqlQuery.hpp"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVBoxLayout>

BillTab::BillTab(QWidget *parent) : QWidget(parent) {
  initUi();
  initModel();
  loadBills();
}

void BillTab::initUi() {
  auto *mainLayout = new QVBoxLayout(this);

  auto *filterLayout = new QHBoxLayout();
  statusFilter = new QComboBox(this);
  statusFilter->addItems({"全部", "已支付", "未支付", "部分支付"});

  filterLayout->addWidget(new QLabel("支付状态:"));
  filterLayout->addWidget(statusFilter);
  filterLayout->addStretch();

  m_table = new QTableView(this);
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_table->verticalHeader()->setVisible(false);

  mainLayout->addLayout(filterLayout);
  mainLayout->addWidget(m_table);

  connect(statusFilter, &QComboBox::currentTextChanged, this,
          &BillTab::onStatusFilterChanged);
}

void BillTab::initModel() {
  m_model = new QStandardItemModel(0, 8, this);

  m_model->setHeaderData(0, Qt::Horizontal, "账单ID");
  m_model->setHeaderData(1, Qt::Horizontal, "房费");
  m_model->setHeaderData(2, Qt::Horizontal, "消费金额");
  m_model->setHeaderData(3, Qt::Horizontal, "其它费用");
  m_model->setHeaderData(4, Qt::Horizontal, "优惠");
  m_model->setHeaderData(5, Qt::Horizontal, "总金额");
  m_model->setHeaderData(6, Qt::Horizontal, "支付状态");
  m_model->setHeaderData(7, Qt::Horizontal, "创建时间");

  m_proxy = new QSortFilterProxyModel(this);
  m_proxy->setSourceModel(m_model);
  m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

  m_table->setModel(m_proxy);
}

void BillTab::loadBills() {
  m_model->removeRows(0, m_model->rowCount());

  QSqlQuery &q = SqlQuery::instance().query();
  QString sql = R"(
    SELECT
      billId,
      roomCharge,
      consumptionCharge,
      otherCharge,
      discountAmount,
      totalAmount,
      paymentStatus,
      createdTime
    FROM Bill
    ORDER BY createdTime DESC
  )";

  if (!q.exec(sql)) {
    QMessageBox::warning(this, "错误", "账单数据加载失败");
    return;
  }

  while (q.next()) {
    QStringList row;
    for (int i = 0; i < 8; ++i)
      row << q.value(i).toString();
    addRow(row);
  }
}

void BillTab::addRow(const QStringList &row) {
  QList<QStandardItem *> items;
  for (const auto &text : row) {
    auto *item = new QStandardItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    item->setEditable(false);
    items << item;
  }
  m_model->appendRow(items);
}

void BillTab::onStatusFilterChanged(const QString &text) {
  if (text == "全部") {
    m_proxy->setFilterKeyColumn(-1);
    m_proxy->setFilterRegularExpression("");
  } else {
    m_proxy->setFilterKeyColumn(6); // paymentStatus
    m_proxy->setFilterFixedString(text);
  }
}
