#include "EmployeeTab.hpp"
#include "SqlQuery.hpp"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QSet>
#include <QSqlQuery>
#include <QVBoxLayout>

EmployeeTab::EmployeeTab(QWidget *parent) : QWidget(parent) {
  initUi();
  initModel();
  loadEmployees();
}

void EmployeeTab::initUi() {
  auto *mainLayout = new QVBoxLayout(this);

  auto *filterLayout = new QHBoxLayout();

  deptFilter = new QComboBox(this);
  positionFilter = new QComboBox(this);

  deptFilter->addItem("全部部门");
  positionFilter->addItem("全部职位");

  filterLayout->addWidget(new QLabel("部门:"));
  filterLayout->addWidget(deptFilter);
  filterLayout->addWidget(new QLabel("职位:"));
  filterLayout->addWidget(positionFilter);
  filterLayout->addStretch();

  auto *btnLayout = new QHBoxLayout();

  addBtn = new QPushButton("新增员工", this);
  editBtn = new QPushButton("编辑员工", this);
  delBtn = new QPushButton("删除员工", this);

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

  connect(addBtn, &QPushButton::clicked, this, &EmployeeTab::onAddEmployee);
  connect(editBtn, &QPushButton::clicked, this, &EmployeeTab::onEditEmployee);
  connect(delBtn, &QPushButton::clicked, this, &EmployeeTab::onDeleteEmployee);
}

void EmployeeTab::initModel() {
  m_model = new QStandardItemModel(0, 7, this);

  m_model->setHeaderData(0, Qt::Horizontal, "姓名");
  m_model->setHeaderData(1, Qt::Horizontal, "性别");
  m_model->setHeaderData(2, Qt::Horizontal, "手机号");
  m_model->setHeaderData(3, Qt::Horizontal, "职位");
  m_model->setHeaderData(4, Qt::Horizontal, "部门");
  m_model->setHeaderData(5, Qt::Horizontal, "入职日期");
  m_model->setHeaderData(6, Qt::Horizontal, "权限");

  m_proxy = new QSortFilterProxyModel(this);
  m_proxy->setSourceModel(m_model);
  m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

  m_table->setModel(m_proxy);

  connect(deptFilter, &QComboBox::currentTextChanged, this,
          [=](const QString &text) {
            if (text == "全部部门")
              m_proxy->setFilterRegularExpression("");
            else {
              m_proxy->setFilterKeyColumn(4);
              m_proxy->setFilterRegularExpression(text);
            }
          });

  connect(positionFilter, &QComboBox::currentTextChanged, this,
          [=](const QString &text) {
            if (text == "全部职位")
              m_proxy->setFilterRegularExpression("");
            else {
              m_proxy->setFilterKeyColumn(3);
              m_proxy->setFilterRegularExpression(text);
            }
          });
}

void EmployeeTab::loadEmployees() {
  m_model->removeRows(0, m_model->rowCount());
  deptFilter->clear();
  positionFilter->clear();

  deptFilter->addItem("全部部门");
  positionFilter->addItem("全部职位");

  QSet<QString> depts, positions;

  QSqlQuery &q = SqlQuery::instance().query();
  QString sql = R"(
    SELECT name, gender, phone, position, department, hireDate, permissionLevel
    FROM Employee
  )";

  if (!q.exec(sql)) {
    QMessageBox::warning(this, "错误", "员工数据加载失败");
    return;
  }

  while (q.next()) {
    QStringList row;
    for (int i = 0; i < 7; ++i)
      row << q.value(i).toString();

    addRow(row);

    depts.insert(row[4]);
    positions.insert(row[3]);
  }

  for (auto &d : depts)
    deptFilter->addItem(d);
  for (auto &p : positions)
    positionFilter->addItem(p);
}

void EmployeeTab::addRow(const QStringList &row) {
  QList<QStandardItem *> items;
  for (const auto &text : row) {
    auto *item = new QStandardItem(text);
    item->setEditable(false);
    item->setTextAlignment(Qt::AlignCenter);
    items << item;
  }
  m_model->appendRow(items);
}

void EmployeeTab::onAddEmployee() {
  bool ok;
  QString name = QInputDialog::getText(this, "新增员工",
                                       "姓名:", QLineEdit::Normal, "", &ok);
  if (!ok || name.isEmpty())
    return;

  QSqlQuery q;
  q.prepare(R"(
    INSERT INTO Employee
    (name, gender, phone, position, department, hireDate, username, password, permissionLevel)
    VALUES (?, '男', '00000000000', '普通员工', '未分配', CURDATE(), ?, '123456', '普通员工')
  )");
  q.addBindValue(name);
  q.addBindValue(name.toLower());

  if (!q.exec()) {
    QMessageBox::warning(this, "失败", "新增员工失败");
    return;
  }

  loadEmployees();
}

void EmployeeTab::onDeleteEmployee() {
  auto index = m_table->currentIndex();
  if (!index.isValid())
    return;

  int row = m_proxy->mapToSource(index).row();
  QString phone = m_model->item(row, 2)->text();

  if (QMessageBox::question(this, "确认", "确定删除该员工？") !=
      QMessageBox::Yes)
    return;

  QSqlQuery q;
  q.prepare("DELETE FROM Employee WHERE phone = ?");
  q.addBindValue(phone);
  q.exec();

  loadEmployees();
}

void EmployeeTab::onEditEmployee() {
  auto index = m_table->currentIndex();
  if (!index.isValid())
    return;

  int row = m_proxy->mapToSource(index).row();
  QString phone = m_model->item(row, 2)->text();

  bool ok;
  QString newPos = QInputDialog::getText(this, "编辑职位",
                                         "新职位:", QLineEdit::Normal, "", &ok);
  if (!ok || newPos.isEmpty())
    return;

  QSqlQuery q;
  q.prepare("UPDATE Employee SET position = ? WHERE phone = ?");
  q.addBindValue(newPos);
  q.addBindValue(phone);
  q.exec();

  loadEmployees();
}
