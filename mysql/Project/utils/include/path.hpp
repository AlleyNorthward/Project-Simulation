#ifndef PATH_HPP
#define PATH_HPP

#include <QDir>
#include <QString>

namespace utils {
inline QString get_project_path() { return QDir::currentPath(); }

inline QString get_current_path(const char *file) {
  QString filePath = QString(file);
  QString dirPath = QFileInfo(filePath).absolutePath();
  return dirPath;
}

inline QString get_file_path(const QString &path) {
  QFileInfo info(path);
  if (info.isAbsolute())
    return info.absoluteFilePath();

  return QDir(QDir::currentPath()).filePath(path);
}
} // namespace utils
#endif
