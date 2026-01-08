#include "out.hpp"
#include "path.hpp"
#include <QApplication>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QDebug>

// -------------------- LoginPage --------------------
class LoginPage : public QWidget {
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent=nullptr) : QWidget(parent) {
        auto *v = new QVBoxLayout(this);

        auto *label = new QLabel("请登录：", this);
        v->addWidget(label);

        username = new QLineEdit(this);
        username->setPlaceholderText("用户名");
        v->addWidget(username);

        password = new QLineEdit(this);
        password->setPlaceholderText("密码");
        password->setEchoMode(QLineEdit::Password);
        v->addWidget(password);

        auto *btnLogin = new QPushButton("登录", this);
        v->addWidget(btnLogin);

        v->addStretch();

        connect(btnLogin, &QPushButton::clicked, this, [this]() {
            // 测试账号：admin / 123
            if (username->text() == "admin" && password->text() == "123") {
                emit loginSucceeded();
            } else {
                qDebug() << "登录失败！";
            }
        });
    }

signals:
    void loginSucceeded();

private:
    QLineEdit *username;
    QLineEdit *password;
};

// -------------------- MainPage --------------------
class MainPage : public QWidget {
    Q_OBJECT
public:
    explicit MainPage(QWidget *parent=nullptr) : QWidget(parent) {
        auto *v = new QVBoxLayout(this);

        auto *label = new QLabel("内部页面（已登录）", this);
        v->addWidget(label);

        auto *btnLogout = new QPushButton("注销", this);
        v->addWidget(btnLogout);

        v->addStretch();

        connect(btnLogout, &QPushButton::clicked, this, &MainPage::requestLogout);
    }

signals:
    void requestLogout();
};

// -------------------- 安全淡入动画函数 --------------------
static void fadeInWidget(QWidget *w, int duration = 300) {
    // 如果 widget 没有 effect，就创建
    QGraphicsOpacityEffect *eff = qobject_cast<QGraphicsOpacityEffect*>(w->graphicsEffect());
    if (!eff) {
        eff = new QGraphicsOpacityEffect(w);
        w->setGraphicsEffect(eff);
    }

    // 初始化透明度
    eff->setOpacity(0.0);

    // 创建动画
    auto *anim = new QPropertyAnimation(eff, "opacity", w);
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

// -------------------- main --------------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 创建 QStackedWidget 管理页面
    QStackedWidget stacked;

    // 创建页面
    LoginPage *login = new LoginPage;
    MainPage *mainp = new MainPage;

    stacked.addWidget(login);   // index 0
    stacked.addWidget(mainp);   // index 1

    // 登录成功 → 切换到内部页面
    QObject::connect(login, &LoginPage::loginSucceeded, &stacked, [&stacked, mainp]() {
        stacked.setCurrentWidget(mainp);   // 切换显示
        fadeInWidget(mainp, 400);          // 安全淡入动画
    });

    // 注销 → 回到登录页面
    QObject::connect(mainp, &MainPage::requestLogout, &stacked, [&stacked, login]() {
        stacked.setCurrentWidget(login);
        fadeInWidget(login, 300);
    });

    stacked.resize(800, 600);
    stacked.show();

    return app.exec();
}

#include "auto_test.moc"












