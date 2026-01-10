#include "OrderTab.hpp"
#include <QLabel>
#include <QVBoxLayout>

OrderTab::OrderTab(QWidget *parent) : QWidget(parent) {
  auto *v = new QVBoxLayout(this);
  v->addWidget(new QLabel("订单列表"));
  v->addStretch();
}
