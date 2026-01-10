#ifndef HOMETAB_HPP
#define HOMETAB_HPP

#include <QLabel>
#include <QWidget>

class _HomeTitle : public QWidget {
  Q_OBJECT
public:
  explicit _HomeTitle(QWidget *parent = nullptr);
  inline QLabel &get_subTitle() { return *subTitle; }

private:
  QLabel *subTitle;
};

class HomeTab : public QWidget {
  Q_OBJECT
public:
  explicit HomeTab(QWidget *parent = nullptr);
  inline void setUserName(const QString &usr) { username = usr; subTitle->setText(usr);}

private:
  void updateTitle();

private:
  _HomeTitle *homeTitle;
  QString username;
  QLabel *subTitle;
};

#endif
