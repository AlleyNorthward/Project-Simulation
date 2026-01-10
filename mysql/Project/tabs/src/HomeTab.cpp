#include "HomeTab.hpp"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "out.hpp"
// #include "LoginWindow.hpp"
// #include "SqlQuery.hpp"

_HomeTitle::_HomeTitle(QWidget* parent):QWidget(parent){
  auto* v = new QVBoxLayout(this);
  auto* title = new QLabel("欢迎使用酒店管理系统");
  title->setAlignment(Qt::AlignCenter);
  title->setStyleSheet("font-size:24px; font-weight:bold;");

  subTitle = new QLabel("");
  subTitle->setAlignment(Qt::AlignCenter);
  subTitle->setStyleSheet("color:gray;");

  v->addWidget(title);
  v->addWidget(subTitle);
}

HomeTab::HomeTab(QWidget *parent) : QWidget(parent) {
  auto *mainLayout = new QVBoxLayout(this);
  mainLayout->setSpacing(20);
  mainLayout->setContentsMargins(30, 20, 30, 20);

  homeTitle = new _HomeTitle;
  mainLayout->addWidget(homeTitle);
  subTitle = &homeTitle->get_subTitle();

  /* ===== 数据概览卡片 ===== */
  auto *cardLayout = new QHBoxLayout;
  cardLayout->setSpacing(15);

  auto createCard = [](const QString &title, const QString &value) {
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

    auto *t = new QLabel(title);
    t->setStyleSheet("color:#555555;");

    l->addWidget(v);
    l->addWidget(t);
    return w;
  };

  cardLayout->addWidget(createCard("今日入住", "12"));
  cardLayout->addWidget(createCard("今日退房", "8"));
  cardLayout->addWidget(createCard("在住房间", "56"));
  cardLayout->addWidget(createCard("空闲房间", "24"));

  mainLayout->addLayout(cardLayout);

  /* ===== 功能按钮区 ===== */
  auto *btnLayout = new QHBoxLayout;
  btnLayout->setSpacing(20);

  auto createButton = [](const QString &text) {
    auto *b = new QPushButton(text);
    b->setMinimumHeight(40);
    b->setStyleSheet("QPushButton {"
                     "font-size:16px;"
                     "border-radius:6px;"
                     "padding:6px 15px;"
                     "}");
    return b;
  };

  btnLayout->addStretch();
  btnLayout->addWidget(createButton("办理入住"));
  btnLayout->addWidget(createButton("办理退房"));
  btnLayout->addWidget(createButton("房态管理"));
  btnLayout->addWidget(createButton("客户管理"));
  btnLayout->addStretch();

  mainLayout->addLayout(btnLayout);

  /* ===== 底部提示 ===== */
  auto *tip = new QLabel("提示：今日有 3 个房间需要打扫");
  tip->setAlignment(Qt::AlignCenter);
  tip->setStyleSheet("color:#aa0000;");

  mainLayout->addWidget(tip);
  mainLayout->addStretch();
}
