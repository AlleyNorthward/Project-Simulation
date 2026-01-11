#ifndef IMAGESLIDER_HPP
#define IMAGESLIDER_HPP

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QTimer>

class ImageSlider : public QWidget {
  Q_OBJECT
public:
  explicit ImageSlider(const QString& imageDir, QWidget *parent = nullptr);

private:
  void setupSlider();
  void setupAllPictures();
  void updateImage();
  void showEvent(QShowEvent* event);

private slots:
  void nextImage();

private:
  QString imageDir;
  QVector<QPixmap> pixmaps;
  QLabel* imageLabel;
  QTimer* timer;
  QStringList images;
  int currentIndex;
};

#endif
