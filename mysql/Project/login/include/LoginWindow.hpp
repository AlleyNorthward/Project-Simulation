#ifndef LOGINWINDOW_HPP
#define LOGINWINDOW_HPP

#include <QLabel>
#include <QTimer>
#include <QWidget>

class LoginWindow : public QWidget {
  Q_OBJECT
public:
  explicit LoginWindow(QWidget *parent = nullptr);
  ~LoginWindow();

private:
  void setupWindow();
  void setupSlider();

private slots:
  void nextImage();

private:
  QLabel *imageLabel;
  QTimer *timer;
  QStringList images;
  int currentIndex;
};

#endif // LOGINWINDOW_H
