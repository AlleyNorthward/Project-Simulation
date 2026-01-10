#include "OrderTab.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  OrderTab r;
  r.resize(1300, 660);
  r.show();
  return app.exec();
}
