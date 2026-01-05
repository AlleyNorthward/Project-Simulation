#ifndef OUT_HPP
#define OUT_HPP

#include <QIODevice>
#include <QTextStream>

namespace utils {
inline const char *endl = "\n";

inline QTextStream &out() {
  static QTextStream ts(stdout, QIODevice::WriteOnly);
  ts.setEncoding(QStringConverter::Utf8);
  return ts;
}
} // namespace utils

#endif
