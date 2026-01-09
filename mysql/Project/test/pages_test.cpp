#include "ShowPages.hpp"
// #include "MainPage.hpp"
#include <QApplication>

int main(int argc, char* argv[]){
  QApplication app(argc, argv);

  ShowPages pages;
  pages.show();
  // MainPage m;
  // m.show();

  return app.exec();
}











