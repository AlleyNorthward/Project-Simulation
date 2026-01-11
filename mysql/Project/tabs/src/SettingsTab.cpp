#include "SettingsTab.hpp"
#include <QLabel>
#include <QVBoxLayout>

SettingsTab::SettingsTab(QWidget *parent) : QWidget(parent) {
  auto *v = new QVBoxLayout(this);
  v->addWidget(new QLabel("系统设置", this));
  v->addStretch();
}
