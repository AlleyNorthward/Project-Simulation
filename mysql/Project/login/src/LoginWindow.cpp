#include "LoginWindow.hpp"
#include "path.hpp"
#include "out.hpp"
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
}

void LoginWindow::setupSlider() {
  imageLabel = new QLabel(this);
  imageLabel->setAlignment(Qt::AlignCenter);
  imageLabel->setFixedSize(600, 300);

  QDir dir(QDir::currentPath() + "/assets/login");
  images << dir.filePath("1.png") << dir.filePath("2.png")
         << dir.filePath("3.png") << dir.filePath("4.png");

  if (!images.isEmpty()) {
    imageLabel->setPixmap(QPixmap(images[this->currentIndex])
                              .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation));
  }

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &LoginWindow::nextImage);
  timer->start(3000);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(imageLabel);
  setLayout(layout);
}

void LoginWindow::nextImage() {
  if (images.isEmpty())
    return;
  currentIndex = (currentIndex + 1) % images.size();
  imageLabel->setPixmap(QPixmap(images[currentIndex])
                            .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}
