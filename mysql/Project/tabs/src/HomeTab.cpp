#include "HomeTab.hpp"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

HomeTab::HomeTab(QWidget *parent) : QWidget(parent) {
  auto *v = new QVBoxLayout(this);
  auto *h = new QHBoxLayout;

  auto *title = new QLabel("<h2>欢迎使用酒店管理系统</h2>");
  h->addStretch();
  h->addWidget(title);
  h->addStretch();

  auto *hh = new QHBoxLayout;
  hh->addStretch();
  hh->addWidget(new QLabel("概览: 今日入住 X 间, 空闲 Y 间"));
  hh->addStretch();

  v->addLayout(h);
  v->addLayout(hh);
}
