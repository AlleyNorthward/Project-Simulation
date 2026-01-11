#ifndef ROOMSTAB_HPP
#define ROOMSTAB_HPP

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTableView>
#include <QWidget>

class _RoomSearch : public QWidget {
  Q_OBJECT
public:
  explicit _RoomSearch(QWidget *parent = nullptr);

public:
  inline QLineEdit *getSearch() const { return m_search; }
  inline QComboBox *getFilter() const { return m_filter; }
  inline QPushButton *getRefresh() const { return m_refresh; }

private:
  QLineEdit *m_search;
  QComboBox *m_filter;
  QPushButton *m_refresh;
};

class _RoomTable : public QWidget {
  Q_OBJECT
public:
  explicit _RoomTable(QWidget *parent = nullptr);

public:
  inline QSortFilterProxyModel &getProxy() const { return *m_proxy; }
  inline QStandardItemModel &getModel() const { return *m_model; }
  void getDatabaseInfos();

private:
  void addRow(const QString &r, const QString &t, const QString &p,
              const QString &s, const QString &note);

private:
  QTableView *m_table;
  QStandardItemModel *m_model;
  QSortFilterProxyModel *m_proxy;
};

class RoomsTab : public QWidget {
  Q_OBJECT
public:
  explicit RoomsTab(QWidget *parent = nullptr);

private slots:
  void onSearch(const QString &text);
  void onFilter(int idx);
  void onRefresh();

private:
  _RoomSearch *roomSearch;
  _RoomTable *roomTable;
};

#endif
