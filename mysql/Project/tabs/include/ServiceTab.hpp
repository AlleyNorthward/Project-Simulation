#ifndef SERVICETAB_HPP
#define SERVICETAB_HPP

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QPushButton>

class ServiceTab : public QWidget {
  Q_OBJECT
public:
  explicit ServiceTab(QWidget *parent = nullptr);

private:
  void initUi();
  void initModel();
  void loadServices();
  void addRow(const QStringList &row);

private slots:
  void onAddService();
  void onEditService();
  void onDeleteService();
  void onFilterChanged(const QString &text);

private:
  QTableView *m_table;
  QStandardItemModel *m_model;
  QSortFilterProxyModel *m_proxy;

  QComboBox *statusFilter;

  QPushButton *addBtn;
  QPushButton *editBtn;
  QPushButton *delBtn;
};

#endif












