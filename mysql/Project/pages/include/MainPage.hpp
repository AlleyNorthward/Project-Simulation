#ifndef MAINPAGE_HPP
#define MAINPAGE_HPP

#include "BookingTab.hpp"
#include "HomeTab.hpp"
#include "OrderTab.hpp"
#include "RoomsTab.hpp"
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
  SettingsTab *settings;
};
#endif
