#ifndef LOGINWINDOW_HPP
#define LOGINWINDOW_HPP

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
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
  QPushButton *btnLogin;
  QPushButton *btnRegister;

public:
  inline const QLineEdit &get_username() const { return *username; }
  inline const QLineEdit &get_password() const { return *password; }
  inline const QPushButton &get_btnLogin() const { return *btnLogin; }
  inline const QPushButton &get_btnRegister() const { return *btnRegister; }

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
  void attach();

private slots:
  void nextImage();

private:
  QVector<QPixmap> pixmaps;
  QLabel *imageLabel;
  _QLogin *inputWidget;
  QTimer *timer;
  QStringList images;
  int currentIndex;
signals:
  void loginSucceeded(const QString &usr);
};

#endif
