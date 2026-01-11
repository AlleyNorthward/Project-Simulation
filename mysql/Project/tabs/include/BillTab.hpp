#ifndef BILLTAB_HPP
#define BILLTAB_HPP

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QComboBox>

class BillTab : public QWidget {
  Q_OBJECT
public:
  explicit BillTab(QWidget *parent = nullptr);

private:
  void initUi();
  void initModel();
  void loadBills();
  void addRow(const QStringList &row);

private slots:
  void onStatusFilterChanged(const QString &text);

private:
  QTableView *m_table;
  QStandardItemModel *m_model;
  QSortFilterProxyModel *m_proxy;

  QComboBox *statusFilter;
};

#endif












