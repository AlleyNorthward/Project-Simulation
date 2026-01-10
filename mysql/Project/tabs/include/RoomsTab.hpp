#ifndef ROOMSTAB_HPP
#define ROOMSTAB_HPP

#include <QComboBox>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTableView>
#include <QWidget>

class RoomsTab : public QWidget {
  Q_OBJECT
public:
  explicit RoomsTab(QWidget *parent = nullptr);

private slots:
  void onSearch(const QString &text);
  void onFilter(int idx);
  void onRefresh();

private:
  void addRow(const QString &r, const QString &t, const QString &p,
              const QString &s, const QString &note);

private:
  QLineEdit *m_search;
  QComboBox *m_filter;
  QTableView *m_table;
  QStandardItemModel *m_model;
  QSortFilterProxyModel *m_proxy;
};

#endif
