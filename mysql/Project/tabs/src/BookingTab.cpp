#include "BookingTab.hpp"
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

BookingTab::BookingTab(QWidget *parent) {
  auto *form = new QFormLayout(this);
  form->addRow("客户姓名:", new QLineEdit);
  form->addRow("入住日期:", new QLineEdit);
  form->addRow("退房日期:", new QLineEdit);
  form->addRow("房型:", new QComboBox);
  form->addRow("备注:", new QTextEdit);
  form->addRow(new QLabel(""), new QPushButton("提交预订"));
}
