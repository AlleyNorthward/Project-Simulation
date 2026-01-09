#ifndef MAINPAGE_HPP
#define MAINPAGE_HPP

#include <QMainWindow>
#include <QTabWidget>
#include <QLabel>

class MainPage : public QMainWindow {
  Q_OBJECT
public:
  MainPage(QWidget *parent = nullptr);

private:
  void setupHome();
  void setupRooms();
  void setupBooking();
  void setupOrders();
  void setupSettings();
  void setupTabs();
  void addTabs();

signals:
  void requestLogout();

private:
  QTabWidget* tabs;
  QWidget* home;
  QWidget* rooms;
  QWidget* booking;
  QWidget* orders;
  QWidget* settings;
};
#endif
