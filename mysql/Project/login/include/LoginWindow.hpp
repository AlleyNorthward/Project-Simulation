#ifndef LOGINWINDOW_HPP
#define LOGINWINDOW_HPP

#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QShowEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class _QLogin : public QWidget {
  Q_OBJECT
public:
  explicit _QLogin(QWidget *parent = nullptr);

private:
  QLineEdit *username;
  QLineEdit *password;
  QFrame *frame;

private:
  void setupFrame();
  void setupOthers();
};

class LoginWindow : public QWidget {
  Q_OBJECT
public:
  explicit LoginWindow(QWidget *parent = nullptr);
  ~LoginWindow();

private:
  void setupWindow();
  void setupSlider();
  void setupInput();
  void setupAllPictures();
  void updateImage();
  void showEvent(QShowEvent *event);

private slots:
  void nextImage();

private:
  QVector<QPixmap> pixmaps;
  QLabel *imageLabel;
  _QLogin *inputWidget;
  QTimer *timer;
  QStringList images;
  int currentIndex;
};

#endif // LOGINWINDOW_H
