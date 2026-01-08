#include "out.hpp"
// #include "path.hpp"
#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>

int main(int argc, char* argv[]){
  QApplication a(argc, argv);

  QWidget p;
  p.resize(1300, 660);
  p.setWindowTitle("测试系统");
  p.show();

  QLabel* l = new QLabel(&p);
  l->setAlignment(Qt::AlignCenter);
  l->setFixedSize(600, 300);
  l->setStyleSheet("background:red;");

  QVBoxLayout* layout = new QVBoxLayout(&p);
  layout->setAlignment(Qt::AlignCenter);
  layout->addWidget(l);
  p.setLayout(layout);
  return a.exec();
}

