#include "anim.hpp"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

void fadeInWidget(QWidget *w, int duration) {
  QGraphicsOpacityEffect *eff =
      qobject_cast<QGraphicsOpacityEffect *>(w->graphicsEffect());
  if (!eff) {
    eff = new QGraphicsOpacityEffect(w);
    w->setGraphicsEffect(eff);
  }

  eff->setOpacity(0.0);

  auto *anim = new QPropertyAnimation(eff, "opacity", w);
  anim->setDuration(duration);
  anim->setStartValue(0.0);
  anim->setEndValue(1.0);
  anim->start(QAbstractAnimation::DeleteWhenStopped);
}
