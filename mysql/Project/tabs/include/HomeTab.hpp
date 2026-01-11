#ifndef HOMETAB_HPP
#define HOMETAB_HPP

#include "ImageSlider.hpp"
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

class _HomeTitle : public QWidget {
  Q_OBJECT
public:
  explicit _HomeTitle(QWidget *parent = nullptr);
  void setUserName(const QString &usr);

private:
  void setupTimer();

private slots:
  void updateTime();

private:
  QString username;

private:
  QTimer *timer;
  QLabel *subTitle;
};

class _HomeCard : public QWidget {
  Q_OBJECT
public:
  explicit _HomeCard(QWidget *parent = nullptr);

private:
  QWidget *createCard(const QString &title, const QString &value);
  void getStatusDiscountEligible();
  void getCount();

private:
  QVector<QString> count;
};

class _HomeButton : public QWidget {
  Q_OBJECT
public:
  explicit _HomeButton(QWidget *parent = nullptr);

signals:
  void checkInRequested();      // 办理入住
  void checkOutRequested();     // 办理退房
  void roomStatusRequested();   // 房态管理
  void customerMgmtRequested(); // 客户管理
private:
  QPushButton *createButton(const QString &text);
};

class _HomeTips : public QWidget {
  Q_OBJECT
public:
  explicit _HomeTips(QWidget *parent = nullptr);
};

class HomeTab : public QWidget {
  Q_OBJECT
public:
  explicit HomeTab(QWidget *parent = nullptr);
  inline void setUserName(const QString &usr) { homeTitle->setUserName(usr); }

private:
  _HomeTitle *homeTitle;
  _HomeCard *homeCard;
  _HomeButton *homeButton;
  _HomeTips *homeTips;
  ImageSlider *imageSlider;
};

#endif
