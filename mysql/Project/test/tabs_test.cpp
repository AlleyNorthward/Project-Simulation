#include "HomeTab.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  HomeTab r;
  r.resize(1300, 660);
  r.show();
  return app.exec();
}
