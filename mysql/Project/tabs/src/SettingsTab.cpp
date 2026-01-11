#include "SettingsTab.hpp"

SettingsTab::SettingsTab(QWidget *parent) : QWidget(parent) {
  initUI();
}

void SettingsTab::initUI() {
  auto *mainLayout = new QVBoxLayout(this);

  auto *uiGroup = new QGroupBox("界面设置", this);
  auto *uiLayout = new QFormLayout(uiGroup);

  themeCombo = new QComboBox(this);
  themeCombo->addItems({"浅色", "深色"});
  uiLayout->addRow("主题:", themeCombo);

  fontSizeSpin = new QSpinBox(this);
  fontSizeSpin->setRange(8, 24);
  fontSizeSpin->setValue(12);
  uiLayout->addRow("字体大小:", fontSizeSpin);

  mainLayout->addWidget(uiGroup);

  auto *notifyGroup = new QGroupBox("通知设置", this);
  auto *notifyLayout = new QFormLayout(notifyGroup);

  newOrderNotify = new QCheckBox("新订单提醒", this);
  newOrderNotify->setChecked(true);
  notifyLayout->addRow(newOrderNotify);

  maintenanceNotify = new QCheckBox("维修提醒", this);
  maintenanceNotify->setChecked(true);
  notifyLayout->addRow(maintenanceNotify);

  mainLayout->addWidget(notifyGroup);

  auto *printerGroup = new QGroupBox("打印/收银", this);
  auto *printerLayout = new QFormLayout(printerGroup);

  printerCombo = new QComboBox(this);
  printerCombo->addItems({"默认打印机", "小票机1", "小票机2"});
  printerLayout->addRow("打印机选择:", printerCombo);

  autoBackup = new QCheckBox("自动备份账单", this);
  autoBackup->setChecked(true);
  printerLayout->addRow(autoBackup);

  mainLayout->addWidget(printerGroup);

  auto *securityGroup = new QGroupBox("安全设置", this);
  auto *securityLayout = new QFormLayout(securityGroup);

  changePasswordBtn = new QPushButton("修改系统管理员密码", this);
  securityLayout->addRow(changePasswordBtn);

  mainLayout->addWidget(securityGroup);

  mainLayout->addStretch(1);

  connect(themeCombo, &QComboBox::currentTextChanged,
          this, &SettingsTab::themeChanged);
  connect(newOrderNotify, &QCheckBox::toggled, [=](bool checked) {
    emit notificationsChanged(checked, maintenanceNotify->isChecked());
  });
  connect(maintenanceNotify, &QCheckBox::toggled, [=](bool checked) {
    emit notificationsChanged(newOrderNotify->isChecked(), checked);
  });
  connect(printerCombo, &QComboBox::currentTextChanged,
          this, &SettingsTab::printerChanged);
  connect(changePasswordBtn, &QPushButton::clicked,
          this, &SettingsTab::passwordChangeRequested);
}
