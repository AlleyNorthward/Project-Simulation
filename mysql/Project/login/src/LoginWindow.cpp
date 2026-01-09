#include "LoginWindow.hpp"
#include "out.hpp"
#include "path.hpp"
#include <QDir>
#include <QHBoxLayout>
#include <QIcon>
#include <QLineEdit>
#include <QVBoxLayout>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
  setupAllPictures();
  setupWindow();
  setupSlider();
  setupInput();
  connect(&inputWidget->get_btnLogin(), &QPushButton::clicked, this, &LoginWindow::attach);

  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *h_layout = new QHBoxLayout;
  h_layout->addStretch(1);
  h_layout->addWidget(imageLabel, 10);
  h_layout->addStretch(1);
  layout->addLayout(h_layout, 8);
  layout->addWidget(inputWidget, 2);
}

LoginWindow::~LoginWindow() {
  if (timer)
    timer->stop();
}

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
  // imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  // imageLabel->setFixedHeight(this->height() / 1.3);
  imageLabel->setScaledContents(true);
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  // 这里是debug模式的. 怎么能使能开启或关闭呢?
  // imageLabel->setStyleSheet("background-color: red;"
  //                           "border: 2px solid blue;");
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &LoginWindow::nextImage);
  timer->start(3000);
}

void LoginWindow::nextImage() {
  if (images.isEmpty())
    return;
  currentIndex = (currentIndex + 1) % images.size();
  updateImage();
}

void LoginWindow::updateImage() {
  if (images.isEmpty())
    return;
  // imageLabel->setPixmap(pix.scaled(imageLabel->size(), Qt::KeepAspectRatio,
  //                                  Qt::SmoothTransformation));
  imageLabel->setPixmap(this->pixmaps[this->currentIndex]);
}

void LoginWindow::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);
  this->updateImage();
}

void LoginWindow::setupAllPictures() {
  this->currentIndex = 0;
  QDir dir(QDir::currentPath() + "/assets/login");
  for (int i = 1; i <= 10; i++) {
    images << dir.filePath(QString::number(i) + ".jpg");
  }

  for (const auto &path : images) {
    pixmaps.push_back(QPixmap(path));
  }
}

void LoginWindow::setupInput() { inputWidget = new _QLogin(this); }

_QLogin::_QLogin(QWidget *parent) : QWidget(parent) {
  this->setupFrame();
  this->setupOthers();
}

void _QLogin::setupFrame() {
  auto *v_frame = new QVBoxLayout(this);
  auto *h_frame = new QHBoxLayout;
  frame = new QFrame(this);
  frame->setObjectName("loginFrame");
  frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  frame->setStyleSheet(R"(
        QFrame#loginFrame {
            border: 1px solid #dcdcdc;
            border-radius: 8px;
            background: rgba(255,255,255,0.95);
        }
    )");

  auto *title = new QLabel("山东科技酒店", frame);
  title->setAlignment(Qt::AlignCenter);
  title->setFixedHeight(36);
  title->setStyleSheet(R"(
      QLabel {
          font-size: 36px;
          font-weight: 600;
          color: #303133;
          letter-spacing: 2px;
      }
  )");

  auto *group = new QLabel(" 组长: 张特尔\n\n 组员: 闫凯杰\n\n 组员: 王千龙");
  group->setAlignment(Qt::AlignCenter);
  group->setFixedHeight(136);
  group->setStyleSheet(R"(
      QLabel {
          font-size: 18px;
          font-weight: 600;
          color: #303133;
          letter-spacing: 2px;
      }
  )");

  h_frame->addWidget(title, 2);
  h_frame->addWidget(frame, 1);
  h_frame->addWidget(group, 2);
  v_frame->addLayout(h_frame);
}

void _QLogin::setupOthers() {
  auto *v = new QVBoxLayout(frame);
  auto *label = new QLabel("请登录", frame);
  label->setAlignment(Qt::AlignCenter);
  label->setFixedHeight(36);
  label->setStyleSheet(R"(
      QLabel {
          font-size: 20px;
          font-weight: 600;
          color: #303133;
          letter-spacing: 2px;
      }
  )");
  v->addWidget(label, 0, Qt::AlignCenter);
  username = new QLineEdit(this);
  username->setPlaceholderText("用户名");
  username->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  username->setFixedHeight(32);
  username->setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #ccc;
            border-radius: 6px;
            padding: 6px 8px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 1px solid #409EFF;
        }
    )");
  auto *h_user = new QHBoxLayout;
  h_user->addStretch(1);
  h_user->addWidget(username, 5);
  h_user->addStretch(1);
  v->addLayout(h_user);

  password = new QLineEdit(this);
  password->setPlaceholderText("密码");
  password->setEchoMode(QLineEdit::Password);
  password->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  password->setFixedHeight(32);
  password->setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #ccc;
            border-radius: 6px;
            padding: 6px 8px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 1px solid #409EFF;
        }
    )");
  auto *h_pass = new QHBoxLayout;
  h_pass->addStretch(1);
  h_pass->addWidget(password, 5);
  h_pass->addStretch(1);
  v->addLayout(h_pass);

  btnLogin = new QPushButton("登录", frame);
  btnLogin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  btnLogin->setFixedHeight(32);
  btnRegister = new QPushButton("注册", frame);
  btnRegister->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  btnRegister->setFixedHeight(32);

  auto *h_btn = new QHBoxLayout;
  h_btn->addStretch(9);
  h_btn->addWidget(btnLogin, 2);
  h_btn->addWidget(btnRegister, 2);
  h_btn->addStretch(9);
  v->addLayout(h_btn);
  v->addStretch();
}

void LoginWindow::attach() {
  if (inputWidget->get_username().text() == "admin" && inputWidget->get_password().text() == "123") {
    emit loginSucceeded();
  } else {
    utils::out() << "登录失败" << utils::endl;
  }
}
