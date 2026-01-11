#include "HomeTab.hpp"
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
// #include "out.hpp"
// #include "SqlQuery.hpp"

_HomeTitle::_HomeTitle(QWidget *parent) : QWidget(parent) {
  this->setupTimer();

  auto *v = new QVBoxLayout(this);
  auto *title = new QLabel("欢迎使用酒店管理系统");
  title->setAlignment(Qt::AlignCenter);
  title->setStyleSheet("font-size:24px; font-weight:bold;");

  subTitle = new QLabel("");
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
  auto *h = new QHBoxLayout(this);
  h->setSpacing(15);
  h->addWidget(createCard("今日入住", "12"));
  h->addWidget(createCard("今日退房", "8"));
  h->addWidget(createCard("在住房间", "56"));
  h->addWidget(createCard("空闲房间", "24"));
  h->addWidget(createCard("你好    嘿嘿", "36"));
}

QWidget *_HomeCard::createCard(const QString &title, const QString &value) {
  auto *w = new QWidget;
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
  auto *b = new QPushButton(text);
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
  auto *tip = new QLabel("提示: 今日有 3 个 房间需要打扫");
  tip->setAlignment(Qt::AlignCenter);
  tip->setStyleSheet("color:#aa0000;");
  h->addWidget(tip);
}

HomeTab::HomeTab(QWidget *parent) : QWidget(parent) {
  auto *mainLayout = new QVBoxLayout(this);
  mainLayout->setSpacing(20);
  mainLayout->setContentsMargins(30, 20, 30, 20);

  homeTitle = new _HomeTitle;
  homeCard = new _HomeCard;
  homeButton = new _HomeButton;
  homeTips = new _HomeTips;

  mainLayout->addWidget(homeTitle);
  mainLayout->addWidget(homeCard);
  mainLayout->addWidget(homeButton);
  mainLayout->addWidget(homeTips);

  mainLayout->addStretch();
}
