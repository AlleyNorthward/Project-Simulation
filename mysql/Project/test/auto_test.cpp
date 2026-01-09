#include "out.hpp"
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

class LoginPage : public QWidget {
  Q_OBJECT
public:
  explicit LoginPage(QWidget *parent = nullptr) : QWidget(parent) {
    auto *v = new QVBoxLayout(this);

    auto *label = new QLabel("请登录", this);
    label->setStyleSheet("background:red;");

    v->addWidget(label, 0, Qt::AlignCenter);
    username = new QLineEdit(this);
    username->setPlaceholderText("用户名");
    username->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    username->setFixedHeight(32);
    // username->setFixedSize(200, 32);
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
    auto* h_user = new QHBoxLayout;
    // h_user->addStretch();
    h_user->addStretch();
    h_user->addWidget(username);
    // h_user->addStretch();
    h_user->addStretch();
    v->addLayout(h_user);

    password = new QLineEdit(this);
    password->setPlaceholderText("密码");
    password->setEchoMode(QLineEdit::Password);
    password->setFixedSize(200, 32);
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
    v->addWidget(password, 0, Qt::AlignCenter);

    auto *btnLogin = new QPushButton("登录", this);
    btnLogin->setFixedSize(200, 32);
    v->addWidget(btnLogin, 0, Qt::AlignCenter);

    v->addStretch();
  }

private:
  QLineEdit *username;
  QLineEdit *password;
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  // QStackedWidget stacked;
  LoginPage *login = new LoginPage;
  // stacked.addWidget(login);
  // stacked.resize(800, 600);
  // stacked.show();
  login->resize(300, 300);
  login->show();
  return app.exec();
}
#include "auto_test.moc"
