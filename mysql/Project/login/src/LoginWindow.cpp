#include "LoginWindow.hpp"
#include "out.hpp"
#include "path.hpp"
#include <QDir>
#include <QIcon>
#include <QVBoxLayout>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
  setupWindow();
  setupSlider();
}

LoginWindow::~LoginWindow() {}

void LoginWindow::setupWindow() {
  setWindowTitle("酒店管理系统 - 登录");
  resize(1300, 660);
  QString iconPath = utils::get_file_path("assets/icons/00hotel.png");
  setWindowIcon(QIcon(iconPath));
  utils::out() << "酒店管理系统创建成功!" << utils::endl;
}

void LoginWindow::setupSlider() {
  imageLabel = new QLabel(this);
  imageLabel->setAlignment(Qt::AlignCenter);
  imageLabel->setFixedSize(this->width(), this->height() / 1.3);

  // 这里是debug模式的. 怎么能使能开启或关闭呢?
  imageLabel->setStyleSheet("background-color: red;"
                            "border: 2px solid blue;");

  QDir dir(QDir::currentPath() + "/assets/login");
  images << dir.filePath("1.jpg") << dir.filePath("2.jpg")
         << dir.filePath("3.jpg") << dir.filePath("4.jpg")
         << dir.filePath("5.jpg") << dir.filePath("6.jpg")
         << dir.filePath("7.jpg") << dir.filePath("8.jpg")
         << dir.filePath("9.jpg") << dir.filePath("10.jpg");

  if (!images.isEmpty()) {
    imageLabel->setPixmap(QPixmap(images[this->currentIndex])
                              .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation));
    // imageLabel->setPixmap(QPixmap(images[this->currentIndex]));
  }

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &LoginWindow::nextImage);
  timer->start(3000);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(imageLabel, 0, Qt::AlignHCenter);
  layout->addSpacing(200);
}

void LoginWindow::nextImage() {
  if (images.isEmpty())
    return;
  currentIndex = (currentIndex + 1) % images.size();
  imageLabel->setPixmap(QPixmap(images[this->currentIndex])
                            .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
  // imageLabel->setPixmap(QPixmap(images[this->currentIndex]));
}
