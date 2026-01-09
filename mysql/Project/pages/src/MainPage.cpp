#include "MainPage.hpp"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>

MainPage::MainPage(QWidget* parent):QMainWindow(parent){
  this->setupTabs();
  this->setupHome();
  this->setupRooms();
  this->setupBooking();
  this->setupOrders();
  this->setupSettings();
  this->addTabs();
  setCentralWidget(tabs);
  resize(1300, 660);
}

void MainPage::setupTabs(){
  this->tabs = new QTabWidget;
  tabs->setTabsClosable(false);
  tabs->setMovable(false);
  tabs->setTabPosition(QTabWidget::North);
}

void MainPage::setupHome(){
  home = new QWidget;
  auto* v = new QVBoxLayout(home);
  auto* title = new QLabel("<h2>欢迎使用酒店管理系统</h2>");

  v->addWidget(title);
  v->addWidget(new QLabel("概览: 今日入住 X 间, 空闲 Y 间"));
  v->addStretch();
}

void MainPage::setupRooms(){
  rooms = new QWidget;
  auto* layout = new QVBoxLayout(rooms);
  layout->addWidget(new QLabel("<b>房间页（示例）</b>"));
  layout->addStretch();
}

void MainPage::setupBooking(){
  booking = new QWidget;
  auto* form = new QFormLayout(booking);
  form->addRow("客户姓名:", new QLineEdit);
  form->addRow("入住日期:", new QLineEdit);
  form->addRow("退房日期:", new QLineEdit);
  form->addRow("房型:", new QComboBox);
  form->addRow("备注:", new QTextEdit);
  form->addRow(new QLabel(""), new QPushButton("提交预订"));
}

void MainPage::setupOrders(){
  orders = new QWidget;
  auto *v = new QVBoxLayout(orders);
  v->addWidget(new QLabel("订单列表（示例）"));
  v->addStretch();
}

void MainPage::setupSettings(){
  settings = new QWidget;
  auto *v = new QVBoxLayout(settings);
  v->addWidget(new QLabel("系统设置"));
  v->addStretch();
}

void MainPage::addTabs(){
  tabs->addTab(home, "首页");
  tabs->addTab(rooms, "房间");
  tabs->addTab(booking, "预定");
  tabs->addTab(orders, "订单");
  tabs->addTab(settings, "设置");
}








