#ifndef ORDERTAB_HPP
#define ORDERTAB_HPP
#include <QStandardItemModel>
#include <QTableView>
#include <QWidget>

class OrderTab : public QWidget {
  Q_OBJECT
public:
  explicit OrderTab(QWidget *parent = nullptr);

private:
  void addRow();
  void getDatabaseInfos();

  void addRow(const QString &r, const QString &t, const QString &p,
              const QString &s, const QString &note);

private:
  QTableView *m_table;
  QStandardItemModel *m_model;
};
#endif
