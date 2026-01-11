#include "LoginWindow.hpp"
#include "SqlQuery.hpp"
#include "out.hpp"
#include "path.hpp"
#include <QDir>
#include <QHBoxLayout>
#include <QIcon>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
  setupWindow();
  inputWidget = new _QLogin(this);
  imageSlider = new ImageSlider("/assets/login", this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(imageSlider, 8);
  layout->addWidget(inputWidget, 2);
  connect(&inputWidget->get_btnLogin(), &QPushButton::clicked, this,
          &LoginWindow::attach);
}

void LoginWindow::setupWindow() {
  setWindowTitle("酒店管理系统 - 登录");
  resize(1300, 660);
  QString iconPath = utils::get_file_path("assets/messages/warn.svg");
  setWindowIcon(QIcon(iconPath));
  utils::out() << "酒店管理系统创建成功!" << utils::endl;
}

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
  QString usr = inputWidget->get_username().text();
  QString pwd = inputWidget->get_password().text();

  bool success = false;
  QString queryStr =
      "SELECT username, password FROM Employee WHERE username = ? "
      "AND password = ?";
  QSqlQuery &Query = SqlQuery::instance().query();

  Query.prepare(queryStr);
  Query.bindValue(0, usr);
  Query.bindValue(1, pwd);

  if (Query.exec()) {
    Query.bindValue(0, usr);
    Query.bindValue(1, pwd);

    if (Query.next()) {
      emit loginSucceeded(usr);
      success = true;
    }
    if (!success) {
      QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
  }
}
