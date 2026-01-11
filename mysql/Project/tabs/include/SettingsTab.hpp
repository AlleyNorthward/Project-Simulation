#ifndef SETTINGSTAB_HPP
#define SETTINGSTAB_HPP

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class SettingsTab : public QWidget {
  Q_OBJECT
public:
  explicit SettingsTab(QWidget *parent = nullptr);

signals:
  void themeChanged(const QString &theme);
  void notificationsChanged(bool newOrder, bool maintenance);
  void printerChanged(const QString &printer);
  void passwordChangeRequested();

private:
  QComboBox *themeCombo;
  QSpinBox *fontSizeSpin;
  QCheckBox *newOrderNotify;
  QCheckBox *maintenanceNotify;
  QComboBox *printerCombo;
  QCheckBox *autoBackup;
  QPushButton *changePasswordBtn;

  void initUI();
};

#endif
