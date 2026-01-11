#include "ImageSlider.hpp"
#include <QDir>
#include <QHBoxLayout>

ImageSlider::ImageSlider(const QString &imageDir, QWidget *parent)
    : QWidget(parent), imageDir(imageDir) {
  QHBoxLayout *h = new QHBoxLayout(this);
  setupAllPictures();
  setupSlider();
  h->addStretch(1);
  h->addWidget(imageLabel, 10);
  h->addStretch(1);
}

void ImageSlider::setupAllPictures() {
  this->currentIndex = 0;
  QDir dir(QDir::currentPath() + imageDir);
  for (int i = 1; i <= 10; i++) {
    images << dir.filePath(QString::number(i) + ".jpg");
  }

  for (const auto &path : images) {
    pixmaps.push_back(QPixmap(path));
  }
}

void ImageSlider::setupSlider() {
  imageLabel = new QLabel(this);
  imageLabel->setAlignment(Qt::AlignCenter);
  imageLabel->setScaledContents(true);
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &ImageSlider::nextImage);
  timer->start(3000);
}

void ImageSlider::nextImage() {
  if (images.isEmpty())
    return;
  currentIndex = (currentIndex + 1) % images.size();
  updateImage();
}

void ImageSlider::updateImage() {
  if (images.isEmpty())
    return;
  imageLabel->setPixmap(this->pixmaps[this->currentIndex]);
}

void ImageSlider::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);
  this->updateImage();
}
