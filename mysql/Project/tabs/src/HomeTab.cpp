#include "HomeTab.hpp"
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <SqlQuery.hpp>

_HomeTitle::_HomeTitle(QWidget *parent) : QWidget(parent) {
  this->setupTimer();

  auto *v = new QVBoxLayout(this);
  auto *title = new QLabel("欢迎使用酒店管理系统", this);
  title->setAlignment(Qt::AlignCenter);
  title->setStyleSheet("font-size:24px; font-weight:bold;");

  subTitle = new QLabel("", this);
  subTitle->setAlignment(Qt::AlignCenter);
  subTitle->setStyleSheet("color:gray;");

  v->addWidget(title);
  v->addWidget(subTitle);
}

void _HomeTitle::setupTimer() {
  timer = new QTimer(this);
  timer->setInterval(1000);
  connect(timer, &QTimer::timeout, this, &_HomeTitle::updateTime);
  timer->start();
}

void _HomeTitle::updateTime() {
  QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  subTitle->setText("当前用户: " + username + " | " + time);
}

void _HomeTitle::setUserName(const QString &usr) {
  username = usr;
  QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  subTitle->setText("当前用户: " + usr + " | " + time);
}

_HomeCard::_HomeCard(QWidget *parent) : QWidget(parent) {
  getStatusDiscountEligible();
  auto *h = new QHBoxLayout(this);
  h->setSpacing(15);
  h->addWidget(createCard("空闲", count[0]));
  h->addWidget(createCard("已预订", count[1]));
  h->addWidget(createCard("已入住", count[2]));
  h->addWidget(createCard("维修中", count[3]));
  h->addWidget(createCard("打折房", count[4]));
}

QWidget *_HomeCard::createCard(const QString &title, const QString &value) {
  auto *w = new QWidget(this);
  w->setStyleSheet("QWidget {"
                   "border:1px solid #cccccc;"
                   "border-radius:8px;"
                   "background:#f9f9f9;"
                   "}");

  auto *l = new QVBoxLayout(w);
  l->setAlignment(Qt::AlignCenter);

  auto *v = new QLabel(value);
  v->setStyleSheet("font-size:28px; font-weight:bold;");
  v->setAlignment(Qt::AlignCenter);

  auto *t = new QLabel(title);
  t->setStyleSheet("color:#555555;");
  t->setAlignment(Qt::AlignCenter);

  l->addWidget(v);
  l->addWidget(t);
  return w;
}

_HomeButton::_HomeButton(QWidget *parent) : QWidget(parent) {
  auto h = new QHBoxLayout(this);
  h->addStretch();
  h->addWidget(createButton("办理入住"));
  h->addWidget(createButton("办理退房"));
  h->addWidget(createButton("房态管理"));
  h->addWidget(createButton("客户管理"));
  h->addStretch();
}

QPushButton *_HomeButton::createButton(const QString &text) {
  auto *b = new QPushButton(text, this);
  b->setMinimumHeight(40);
  b->setStyleSheet("QPushButton {"
                   "font-size:16px;"
                   "border-radius:6px;"
                   "padding:6px 15px;"
                   "}");
  return b;
}

_HomeTips::_HomeTips(QWidget *parent) : QWidget(parent) {
  auto h = new QHBoxLayout(this);
  auto *tip = new QLabel("提示: 今日有 3 个 房间需要打扫", this);
  tip->setAlignment(Qt::AlignCenter);
  tip->setStyleSheet("color:#aa0000;");
  h->addWidget(tip);
}

HomeTab::HomeTab(QWidget *parent) : QWidget(parent) {
  auto *mainLayout = new QVBoxLayout(this);
  mainLayout->setSpacing(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  homeTitle = new _HomeTitle(this);
  homeCard = new _HomeCard(this);
  homeButton = new _HomeButton(this);
  homeTips = new _HomeTips(this);
  imageSlider = new ImageSlider("/assets/home", this);

  mainLayout->addWidget(homeTitle, 0);
  mainLayout->addWidget(homeCard, 0);
  mainLayout->addWidget(homeButton, 0);
  mainLayout->addWidget(imageSlider, 1);
  mainLayout->addWidget(homeTips, 0);
}

void _HomeCard::getStatusDiscountEligible() {
  QSqlQuery &Query = SqlQuery::instance().query();
  QString queryStr = R"(
    SELECT 
        SUM(status='空闲'),
        SUM(status='已预订'),
        SUM(status='已入住'),
        SUM(status='维修中'),
        SUM(discountEligible=1)
    FROM Room;
  )";
  Query.prepare(queryStr);
  if (Query.exec() && Query.next()) {
    count.append(Query.value(0).toString());
    count.append(Query.value(1).toString());
    count.append(Query.value(2).toString());
    count.append(Query.value(3).toString());
    count.append(Query.value(4).toString());
  }
}
