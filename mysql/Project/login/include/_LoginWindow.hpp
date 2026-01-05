#ifndef _LOGINWINDOW_HPP
#define _LOGINWINDOW_HPP

#include <QWidget>
#include <QLabel>
#include <QTimer>

class _LoginWindow : public QWidget{
  Q_OBJECT
public:
  explicit _LoginWindow(QWidget* parent = nullptr);
  ~_LoginWindow();
private:
  void setupWindow();
  void setupSlider();
private slots:
  void nextImage();
private:
  QLabel* imageLabel;
  QTimer* timer;
  QStringList images;
  int currentIndex;
};

#endif












