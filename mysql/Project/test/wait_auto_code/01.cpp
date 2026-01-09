// main.cpp
#include <QApplication>
#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QRegularExpression>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
    auto *central = new QWidget;
    auto *vbox = new QVBoxLayout;

    // top controls: search + filter
    auto *hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("搜索:"));
    searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText("按房号/类型/状态搜索...");
    hbox->addWidget(searchEdit);

    hbox->addWidget(new QLabel("状态筛选:"));
    filterCombo = new QComboBox;
    filterCombo->addItem("全部");
    filterCombo->addItem("空闲");
    filterCombo->addItem("入住");
    filterCombo->addItem("维修");
    hbox->addWidget(filterCombo);

    // optional: refresh or new booking button
    QPushButton *refreshBtn = new QPushButton("刷新");
    hbox->addWidget(refreshBtn);

    vbox->addLayout(hbox);

    // Table view + model
    tableView = new QTableView;
    model = new QStandardItemModel(0, 5, this);
    model->setHeaderData(0, Qt::Horizontal, "房号");
    model->setHeaderData(1, Qt::Horizontal, "类型");
    model->setHeaderData(2, Qt::Horizontal, "价格");
    model->setHeaderData(3, Qt::Horizontal, "状态");
    model->setHeaderData(4, Qt::Horizontal, "备注");

    // sample data
    appendRow("101", "单人房", "100", "空闲", "靠窗");
    appendRow("102", "标准双人", "150", "入住", "VIP 客户");
    appendRow("201", "套房", "300", "维修", "空调维修");
    appendRow("202", "标准双人", "150", "空闲", "");
    appendRow("301", "商务套房", "400", "入住", "公司协议");

    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);
    // 默认按所有列搜索：我们在 filterAcceptsRow 重载前先用简单策略（列0）；
    // 更完整的实现可继承 QSortFilterProxyModel 并实现行级搜索。
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterKeyColumn(0); // 默认按房号列，可按需更改

    tableView->setModel(proxy);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setVisible(false);

    vbox->addWidget(tableView);
    central->setLayout(vbox);
    setCentralWidget(central);
    setWindowTitle("酒店管理 - 客户/列表视图示例");
    resize(800, 600);

    // connections
    connect(searchEdit, &QLineEdit::textChanged, this,
            &MainWindow::onSearchTextChanged);
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFilterChanged);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefresh);
  }

private slots:
  void onSearchTextChanged(const QString &text) {
    // 简单实现：把搜索词应用到第0列（房号）或使用正则表达式
    // 要跨列搜索，建议自定义 QSortFilterProxyModel::filterAcceptsRow。
    QRegularExpression re(text, QRegularExpression::CaseInsensitiveOption);
    proxy->setFilterRegularExpression(re);
    // 若要搜索所有列，建议写个自定义 proxy（示例里为简洁未写）
  }

  void onFilterChanged(int index) {
    QString status = filterCombo->currentText();
    if (status == "全部") {
      proxy->setFilterKeyColumn(-1); // 不限制某列：需要自定义 accept 行才有效
      proxy->setFilterRegularExpression(QRegularExpression()); // 清空
      // 简单替代：如果只想按状态过滤，可以设为状态所在列(3)
      proxy->setFilterKeyColumn(3);
      proxy->setFilterRegularExpression(
          QRegularExpression(".*", QRegularExpression::CaseInsensitiveOption));
    } else {
      proxy->setFilterKeyColumn(3); // 状态列
      proxy->setFilterRegularExpression(QRegularExpression(
          status, QRegularExpression::CaseInsensitiveOption));
    }
  }

  void onRefresh() {
    // placeholder：实际中可从 DB 或 API 重新加载
    // 这里简单演示：清空并重新添加（不改变数据）
    model->removeRows(0, model->rowCount());
    appendRow("101", "单人房", "100", "空闲", "靠窗");
    appendRow("102", "标准双人", "150", "入住", "VIP 客户");
    appendRow("201", "套房", "300", "维修", "空调维修");
    appendRow("202", "标准双人", "150", "空闲", "");
    appendRow("301", "商务套房", "400", "入住", "公司协议");
  }

private:
  void appendRow(const QString &rno, const QString &type, const QString &price,
                 const QString &status, const QString &note) {
    QList<QStandardItem *> items;
    items << new QStandardItem(rno) << new QStandardItem(type)
          << new QStandardItem(price) << new QStandardItem(status)
          << new QStandardItem(note);
    model->appendRow(items);
  }

  QLineEdit *searchEdit;
  QComboBox *filterCombo;
  QTableView *tableView;
  QStandardItemModel *model;
  QSortFilterProxyModel *proxy;
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow w;
  w.show();
  return app.exec();
}
#include "auto_test.moc"
