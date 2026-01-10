#ifndef MAINPAGE_HPP
#define MAINPAGE_HPP

#include "HomeTab.hpp"
#include "RoomsTab.hpp"
#include "BookingTab.hpp"
#include "OrderTab.hpp"
#include "SettingsTab.hpp"
#include <QLabel>
#include <QMainWindow>
#include <QTabWidget>

class MainPage : public QMainWindow {
  Q_OBJECT
public:
  explicit MainPage(QWidget *parent = nullptr);

private:
  // void setupSettings();
  void setupTabs();
  void addTabs();

signals:
  void requestLogout();

private:
  QTabWidget *tabs;
  HomeTab *home;
  RoomsTab *rooms;
  BookingTab* booking;
  OrderTab* orders;
  // QWidget *settings;
  SettingsTab* settings;
};
#endif
