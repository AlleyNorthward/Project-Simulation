#include "_LoginWindow.hpp"
#include <QDir>
#include <QIcon>

void _LoginWindow::setupWindow(){
  setWindowTitle("酒店管理系统-嗯哼");
  resize(1300, 660);

  QDir project_dir(QDir::currentPath());
  QString iconPath = project_dir
}
