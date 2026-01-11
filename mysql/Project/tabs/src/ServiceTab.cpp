#include "ServiceTab.hpp"
#include "SqlQuery.hpp"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVBoxLayout>

ServiceTab::ServiceTab(QWidget *parent) : QWidget(parent) {
  initUi();
  initModel();
  loadServices();
}

void ServiceTab::initUi() {
  auto *mainLayout = new QVBoxLayout(this);

  auto *filterLayout = new QHBoxLayout();
  statusFilter = new QComboBox(this);
  statusFilter->addItems({"全部", "启用", "停用"});

  filterLayout->addWidget(new QLabel("状态筛选:"));
  filterLayout->addWidget(statusFilter);
  filterLayout->addStretch();

  auto *btnLayout = new QHBoxLayout();
  addBtn = new QPushButton("新增服务", this);
  editBtn = new QPushButton("编辑服务", this);
  delBtn = new QPushButton("删除服务", this);

  btnLayout->addWidget(addBtn);
  btnLayout->addWidget(editBtn);
  btnLayout->addWidget(delBtn);
  btnLayout->addStretch();

  m_table = new QTableView(this);
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_table->verticalHeader()->setVisible(false);

  mainLayout->addLayout(filterLayout);
  mainLayout->addLayout(btnLayout);
  mainLayout->addWidget(m_table);

  connect(addBtn, &QPushButton::clicked, this, &ServiceTab::onAddService);
  connect(editBtn, &QPushButton::clicked, this, &ServiceTab::onEditService);
  connect(delBtn, &QPushButton::clicked, this, &ServiceTab::onDeleteService);
  connect(statusFilter, &QComboBox::currentTextChanged, this,
          &ServiceTab::onFilterChanged);
}

void ServiceTab::initModel() {
  m_model = new QStandardItemModel(0, 5, this);

  m_model->setHeaderData(0, Qt::Horizontal, "服务名称");
  m_model->setHeaderData(1, Qt::Horizontal, "价格");
  m_model->setHeaderData(2, Qt::Horizontal, "单位");
  m_model->setHeaderData(3, Qt::Horizontal, "描述");
  m_model->setHeaderData(4, Qt::Horizontal, "状态");

  m_proxy = new QSortFilterProxyModel(this);
  m_proxy->setSourceModel(m_model);
  m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

  m_table->setModel(m_proxy);
}

void ServiceTab::loadServices() {
  m_model->removeRows(0, m_model->rowCount());

  QSqlQuery &q = SqlQuery::instance().query();
  QString sql = R"(
    SELECT name, price, unit, description, status
    FROM Service
    ORDER BY name ASC
  )";

  if (!q.exec(sql)) {
    QMessageBox::warning(this, "错误", "服务数据加载失败");
    return;
  }

  while (q.next()) {
    QStringList row;
    for (int i = 0; i < 5; ++i)
      row << q.value(i).toString();
    addRow(row);
  }
}

void ServiceTab::addRow(const QStringList &row) {
  QList<QStandardItem *> items;
  for (const auto &text : row) {
    auto *item = new QStandardItem(text);
    item->setEditable(false);
    item->setTextAlignment(Qt::AlignCenter);
    items << item;
  }
  m_model->appendRow(items);
}

void ServiceTab::onFilterChanged(const QString &text) {
  if (text == "全部") {
    m_proxy->setFilterRegularExpression("");
    m_proxy->setFilterKeyColumn(-1);
  } else {
    m_proxy->setFilterKeyColumn(4); // 状态列
    m_proxy->setFilterFixedString(text);
  }
}

void ServiceTab::onAddService() {
  bool ok;
  QString name = QInputDialog::getText(this, "新增服务",
                                       "服务名称:", QLineEdit::Normal, "", &ok);
  if (!ok || name.isEmpty())
    return;

  QSqlQuery q;
  q.prepare(R"(
    INSERT INTO Service (name, price, unit, description, status)
    VALUES (?, 0.00, '次', '', '启用')
  )");
  q.addBindValue(name);

  if (!q.exec()) {
    QMessageBox::warning(this, "失败", "新增服务失败");
    return;
  }

  loadServices();
}

void ServiceTab::onEditService() {
  auto index = m_table->currentIndex();
  if (!index.isValid())
    return;

  int row = m_proxy->mapToSource(index).row();
  QString name = m_model->item(row, 0)->text();

  bool ok;
  double price =
      QInputDialog::getDouble(this, "修改价格", "新价格:", 0, 0, 10000, 2, &ok);
  if (!ok)
    return;

  QSqlQuery q;
  q.prepare("UPDATE Service SET price = ? WHERE name = ?");
  q.addBindValue(price);
  q.addBindValue(name);
  q.exec();

  loadServices();
}

void ServiceTab::onDeleteService() {
  auto index = m_table->currentIndex();
  if (!index.isValid())
    return;

  int row = m_proxy->mapToSource(index).row();
  QString name = m_model->item(row, 0)->text();

  if (QMessageBox::question(this, "确认", "确定删除该服务？") !=
      QMessageBox::Yes)
    return;

  QSqlQuery q;
  q.prepare("DELETE FROM Service WHERE name = ?");
  q.addBindValue(name);
  q.exec();

  loadServices();
}
