#ifndef EMPLOYEETAB_HPP
#define EMPLOYEETAB_HPP

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QPushButton>

class EmployeeTab : public QWidget {
  Q_OBJECT
public:
  explicit EmployeeTab(QWidget *parent = nullptr);

private:
  void initUi();
  void initModel();
  void loadEmployees();
  void addRow(const QStringList &row);

private slots:
  void onAddEmployee();
  void onDeleteEmployee();
  void onEditEmployee();

private:
  QTableView *m_table;
  QStandardItemModel *m_model;
  QSortFilterProxyModel *m_proxy;

  QComboBox *deptFilter;
  QComboBox *positionFilter;

  QPushButton *addBtn;
  QPushButton *editBtn;
  QPushButton *delBtn;
};

#endif











