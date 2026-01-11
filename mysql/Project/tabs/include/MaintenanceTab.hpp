#ifndef MAINTENANCETAB_HPP
#define MAINTENANCETAB_HPP

#include <QWidget>
#include <QTableView>
#include <QTextEdit>
#include <QComboBox>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

class MaintenanceTab : public QWidget {
  Q_OBJECT
public:
  explicit MaintenanceTab(QWidget *parent = nullptr);

private:
  void initUi();
  void initModel();
  void loadData();
  void addRow(const QStringList &row);

private slots:
  void onStatusChanged(const QString &status);
  void onRowChanged(const QModelIndex &current);

private:
  QTableView *table;
  QTextEdit *detail;

  QComboBox *statusFilter;

  QStandardItemModel *model;
  QSortFilterProxyModel *proxy;
};

#endif // MAINTENANCETAB_HPP












