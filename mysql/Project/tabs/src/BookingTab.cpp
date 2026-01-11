#include "BookingTab.hpp"
#include "SqlQuery.hpp"
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>

BookingTab::BookingTab(QWidget *parent) {
  auto *form = new QFormLayout(this);

  userName = new QLineEdit(this);
  phoneNumber = new QLineEdit(this);
  checkInDate = new QLineEdit(this);
  checkOutDate = new QLineEdit(this);
  depositAmount = new QLineEdit(this);

  roomType = new QComboBox(this);
  vipType = new QComboBox(this);
  serviceType = new QComboBox(this);
  remark = new QLineEdit(this);
  btn = new QPushButton("提交预定", this);

  form->addRow("客户姓名:", userName);
  form->addRow("手机号:", phoneNumber);
  form->addRow("入住日期:", checkInDate);
  form->addRow("退房日期:", checkOutDate);
  form->addRow("定金金额:", depositAmount);

  form->addRow("房型:", roomType);
  getRoomType();
  form->addRow("会员类型:", vipType);
  getVipType();
  form->addRow("服务类型", serviceType);
  serviceType->addItem("无");
  getServiceType();
  form->addRow("备注:", remark);
  form->addRow(new QLabel(""), btn);
  remark->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  connect(btn, &QPushButton::clicked, this, &BookingTab::warn);
}

void BookingTab::getRoomType() {
  QSqlQuery &Query = SqlQuery::instance().query();
  QString queryStr = R"(
    SELECT name
    FROM RoomType;
  )";
  if (Query.exec(queryStr)) {
    while (Query.next()) {
      roomType->addItem(Query.value(0).toString());
    }
  }
}

void BookingTab::getVipType() {
  QSqlQuery &Query = SqlQuery::instance().query();
  QString queryStr = R"(
    SELECT levelname
    FROM MembershipLevel;
  )";
  if (Query.exec(queryStr)) {
    while (Query.next()) {
      vipType->addItem(Query.value(0).toString());
    }
  }
}

void BookingTab::getServiceType() {
  QSqlQuery &Query = SqlQuery::instance().query();
  QString queryStr = R"(
    SELECT name
    FROM Service;
  )";
  if (Query.exec(queryStr)) {
    while (Query.next()) {
      serviceType->addItem(Query.value(0).toString());
    }
  }
}

void BookingTab::submit() {
  QSqlQuery &Query = SqlQuery::instance().query();

  QString name = userName->text();
  QString phone = phoneNumber->text();
  QString checkIn = checkInDate->text();
  QString checkOut = checkOutDate->text();
  QString deposit = depositAmount->text();
  QString notes = remark->text();
  QString selectedRoom = roomType->currentText();
  QString selectedVip = vipType->currentText();
  QString selectedService = serviceType->currentText();

  Query.prepare(R"(
        INSERT INTO Reservation
        (customerId, roomTypeId, reservedRoomCount, checkInDate, checkOutDate, status, depositAmount, remark)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    )");
}

void BookingTab::warn() {
  QMessageBox::warning(this, "提交失败", "格式不正确");
}
