#include "MainPage.hpp"
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

MainPage::MainPage(QWidget *parent) : QMainWindow(parent) {
  this->setupTabs();
  home = new HomeTab(this);
  rooms = new RoomsTab(this);
  booking = new BookingTab(this);
  orders = new OrderTab(this);
  employee = new EmployeeTab(this);
  service = new ServiceTab(this);
  bill = new BillTab(this);
  maintanance = new MaintenanceTab(this);
  settings = new SettingsTab(this);
  this->addTabs();
  setCentralWidget(tabs);
  resize(1300, 660);
}

void MainPage::setupTabs() {
  this->tabs = new QTabWidget;
  tabs->setTabsClosable(false);
  tabs->setMovable(false);
  tabs->setTabPosition(QTabWidget::North);
}

void MainPage::addTabs() {
  tabs->addTab(home, "首页");
  tabs->addTab(rooms, "房间");
  tabs->addTab(booking, "预定");
  tabs->addTab(orders, "订单");
  tabs->addTab(service, "服务类型");
  tabs->addTab(employee, "员工信息");
  tabs->addTab(bill, "账单");
  tabs->addTab(maintanance, "维修信息");
  tabs->addTab(settings, "设置");
}
