#include "MaintenanceTab.hpp"
#include "SqlQuery.hpp"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVBoxLayout>

MaintenanceTab::MaintenanceTab(QWidget *parent) : QWidget(parent) {
  initUi();
  initModel();
  loadData();
}

void MaintenanceTab::initUi() {
  auto *mainLayout = new QVBoxLayout(this);

  auto *filterLayout = new QHBoxLayout();
  statusFilter = new QComboBox(this);
  statusFilter->addItems({"全部", "进行中", "已完成"});

  filterLayout->addWidget(new QLabel("维修状态:"));
  filterLayout->addWidget(statusFilter);
  filterLayout->addStretch();

  table = new QTableView(this);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  table->setSelectionMode(QAbstractItemView::SingleSelection);
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  table->verticalHeader()->setVisible(false);

  detail = new QTextEdit(this);
  detail->setReadOnly(true);
  detail->setPlaceholderText("选择一条维修记录查看问题描述…");
  detail->setFixedHeight(100);

  mainLayout->addLayout(filterLayout);
  mainLayout->addWidget(table);
  mainLayout->addWidget(new QLabel("问题描述:"));
  mainLayout->addWidget(detail);

  connect(statusFilter, &QComboBox::currentTextChanged, this,
          &MaintenanceTab::onStatusChanged);
  table->setModel(proxy);
  connect(table->selectionModel(), &QItemSelectionModel::currentRowChanged,
          this, &MaintenanceTab::onRowChanged);
}

void MaintenanceTab::initModel() {
  model = new QStandardItemModel(0, 6, this);

  model->setHeaderData(0, Qt::Horizontal, "房间号");
  model->setHeaderData(1, Qt::Horizontal, "维修员工");
  model->setHeaderData(2, Qt::Horizontal, "开始时间");
  model->setHeaderData(3, Qt::Horizontal, "结束时间");
  model->setHeaderData(4, Qt::Horizontal, "状态");
  model->setHeaderData(5, Qt::Horizontal, "描述");

  proxy = new QSortFilterProxyModel(this);
  proxy->setSourceModel(model);
  proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

  table->setModel(proxy);
  table->hideColumn(5);
}

void MaintenanceTab::loadData() {
  model->removeRows(0, model->rowCount());

  QSqlQuery &q = SqlQuery::instance().query();

  QString sql = R"(
    SELECT
      roomId,
      employeeId,
      startTime,
      IFNULL(endTime, '未完成'),
      status,
      problemDescription
    FROM Maintenance
    ORDER BY startTime DESC
  )";

  if (!q.exec(sql)) {
    QMessageBox::warning(this, "错误", "维修数据加载失败");
    return;
  }

  while (q.next()) {
    QStringList row;
    for (int i = 0; i < 6; ++i)
      row << q.value(i).toString();
    addRow(row);
  }
}

void MaintenanceTab::addRow(const QStringList &row) {
  QList<QStandardItem *> items;
  for (const auto &text : row) {
    auto *item = new QStandardItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    item->setEditable(false);
    items << item;
  }
  model->appendRow(items);
}

void MaintenanceTab::onStatusChanged(const QString &status) {
  if (status == "全部") {
    proxy->setFilterKeyColumn(-1);
    proxy->setFilterRegularExpression("");
  } else {
    proxy->setFilterKeyColumn(4);
    proxy->setFilterFixedString(status);
  }
}

void MaintenanceTab::onRowChanged(const QModelIndex &current) {
  if (!current.isValid()) {
    detail->clear();
    return;
  }

  QModelIndex descIndex = proxy->index(current.row(), 5);

  detail->setText(descIndex.data().toString());
}
