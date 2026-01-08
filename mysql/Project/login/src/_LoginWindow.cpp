#include "_LoginWindow.hpp"
#include "path.hpp"
#include "out.hpp"
#include <QDir>
#include <QIcon>

void _LoginWindow::setupWindow(){
  setWindowTitle("酒店管理系统");
  resize(1300, 660);
  const QString iconPath = utils::get_file_path("assets/icons/00hotle.png");
  setWindowIcon(QIcon(iconPath));
  utils::out() << "酒店管理系统窗口创建成功!" << utils::endl;
}

void _LoginWindow::setupSlider(){
  imageLabel = new QLabel(this);
  imageLabel->setAlignment(Qt::AlignCenter);
}











