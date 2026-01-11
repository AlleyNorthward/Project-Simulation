#ifndef BOOKINGTAB_HPP
#define BOOKINGTAB_HPP

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
class BookingTab : public QWidget {
  Q_OBJECT
public:
  explicit BookingTab(QWidget *parent = nullptr);

private:
  void getRoomType();
  void getVipType();
  void getServiceType();

private slots:
  void submit();
  void warn();
private:
  QLineEdit *userName;
  QLineEdit *phoneNumber;
  QLineEdit *checkInDate;
  QLineEdit *checkOutDate;
  QLineEdit *depositAmount;
  QComboBox *roomType;
  QComboBox *vipType;
  QComboBox *serviceType;
  QLineEdit *remark;
  QPushButton* btn;
};

#endif
