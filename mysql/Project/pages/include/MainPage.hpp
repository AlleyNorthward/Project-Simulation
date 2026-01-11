#ifndef MAINPAGE_HPP
#define MAINPAGE_HPP

#include "BillTab.hpp"
#include "BookingTab.hpp"
#include "EmployeeTab.hpp"
#include "HomeTab.hpp"
#include "MaintenanceTab.hpp"
#include "OrderTab.hpp"
#include "RoomsTab.hpp"
#include "ServiceTab.hpp"
#include "SettingsTab.hpp"
#include <QLabel>
#include <QMainWindow>
#include <QTabWidget>

class MainPage : public QMainWindow {
  Q_OBJECT
public:
  explicit MainPage(QWidget *parent = nullptr);
  inline void setUserName(const QString &usr) { home->setUserName(usr); }

private:
  void setupTabs();
  void addTabs();

signals:
  void requestLogout();

private:
  QTabWidget *tabs;
  HomeTab *home;
  RoomsTab *rooms;
  BookingTab *booking;
  OrderTab *orders;
  EmployeeTab *employee;
  ServiceTab *service;
  BillTab *bill;
  MaintenanceTab * maintanance;
  SettingsTab *settings;
};
#endif
