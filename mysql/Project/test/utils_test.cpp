#include "path.hpp"
#include "out.hpp"
#include "ImageSlider.hpp"
#include <QDebug>
#include <QApplication>

int main(int argc, char* argv[]){
  QApplication app(argc, argv);
  QString a = utils::get_current_path(__FILE__);
  QString b = utils::get_project_path();

  utils::out() << a << utils::endl;
  utils::out() << b << utils::endl;
  utils::out() << "你好, 哈哈" << utils::endl;
  utils::out() << "山东科技大学" << utils::endl;
  utils::out() << "你好, 嘿嘿嘿哈" << utils::endl;
  ImageSlider im(QString("assets"));
  return app.exec();
}
