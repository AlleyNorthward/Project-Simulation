#include "RoomsTab.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>

RoomsTab::RoomsTab(QWidget* parent): QWidget(parent){
  auto *v = new QVBoxLayout(this);
  auto* h = new QHBoxLayout;

  h->addWidget(new QLabel("搜索:"));
  m_search = new QLineEdit;
  m_search->setPlaceholderText("按房号/类型/备注搜索");
  h->addWidget(m_search);

  h->addWidget(new QLabel("状态:"));
  m_filter = new QComboBox;
  m_filter->addItems({"全部", "空闲", "入住", "维修"});
  h->addWidget(m_filter);

  QPushButton *refresh = new QPushButton("刷新");
  h->addWidget(refresh);
  v->addLayout(h);

  m_table = new QTableView;
  m_model = new QStandardItemModel(0, 5, this);
  m_model->setHeaderData(0, Qt::Horizontal, "房号");
  m_model->setHeaderData(1, Qt::Horizontal, "类型");
  m_model->setHeaderData(2, Qt::Horizontal, "价格");
  m_model->setHeaderData(3, Qt::Horizontal, "状态");
  m_model->setHeaderData(4, Qt::Horizontal, "备注");

  addRow("101", "单人房", "100", "空闲", "靠窗");
  addRow("102", "标准双人", "150", "入住", "VIP");
  addRow("201", "套房", "300", "维修", "空调");
  addRow("202", "标准双人", "150", "空闲", "");
  addRow("301", "商务套房", "400", "入住", "公司协议");

  m_proxy = new QSortFilterProxyModel(this);
  m_proxy->setSourceModel(m_model);
  m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

  m_proxy->setFilterKeyColumn(-1);

  m_table->setModel(m_proxy);
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_table->verticalHeader()->setVisible(false);

  v->addWidget(m_table);
  connect(m_search, &QLineEdit::textChanged, this, &RoomsTab::onSearch);
  connect(m_filter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RoomsTab::onFilter);
  connect(refresh, &QPushButton::clicked, this, &RoomsTab::onRefresh);
}

void RoomsTab::addRow(const QString &r, const QString &t, const QString &p, const QString &s, const QString &note) {

  QList<QStandardItem*> row;
  row << new QStandardItem(r)
      << new QStandardItem(t)
      << new QStandardItem(p)
      << new QStandardItem(s)
      << new QStandardItem(note);
  m_model->appendRow(row);
}


void RoomsTab::onSearch(const QString &text) {
    // 跨列搜索：构建正则并让 proxy 在所有列匹配
    QRegularExpression re(text, QRegularExpression::CaseInsensitiveOption);
    m_proxy->setFilterRegularExpression(re);
}

void RoomsTab::onFilter(int idx) {
    QString status = m_filter->currentText();
    if (status == "全部") {
        m_proxy->setFilterFixedString(QString()); // 清空状态过滤（这里保留搜索）
        // 保证 proxy 对所有列生效
        m_proxy->setFilterKeyColumn(-1);
    } else {
        // 临时方案：仅针对状态列(3)进行额外过滤 — 与全列搜索兼容需要自定义 proxy
        m_proxy->setFilterKeyColumn(3);
        m_proxy->setFilterFixedString(status);
    }
}

void RoomsTab::onRefresh() {
    // 简单刷新示例 — 真正项目中这里从 DB 或 API 重新加载
    m_model->removeRows(0, m_model->rowCount());
    addRow("101", "单人房", "100", "空闲", "靠窗");
    addRow("102", "标准双人", "150", "入住", "VIP");
    addRow("201", "套房", "300", "维修", "空调");
    addRow("202", "标准双人", "150", "空闲", "");
    addRow("301", "商务套房", "400", "入住", "公司协议");
}









