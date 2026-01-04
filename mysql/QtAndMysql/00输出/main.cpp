#include <QCoreApplication>
#include <QTextStream>

int main(int argc, char *argv[]){
  QCoreApplication a(argc, argv);
  QTextStream out(stdout);

  out << "你好, 嘿嘿嘿" << "\n";
  return 0;
}
