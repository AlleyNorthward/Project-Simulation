#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QFormLayout>
#include <QTextEdit>

class RoomsWidget : public QWidget {
    Q_OBJECT
public:
    RoomsWidget(QWidget* parent = nullptr) : QWidget(parent) {
        auto *v = new QVBoxLayout(this);

        auto *h = new QHBoxLayout;
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

        // table view
        m_table = new QTableView;
        m_model = new QStandardItemModel(0, 5, this);
        m_model->setHeaderData(0, Qt::Horizontal, "房号");
        m_model->setHeaderData(1, Qt::Horizontal, "类型");
        m_model->setHeaderData(2, Qt::Horizontal, "价格");
        m_model->setHeaderData(3, Qt::Horizontal, "状态");
        m_model->setHeaderData(4, Qt::Horizontal, "备注");

        // demo 数据
        addRow("101", "单人房", "100", "空闲", "靠窗");
        addRow("102", "标准双人", "150", "入住", "VIP");
        addRow("201", "套房", "300", "维修", "空调");
        addRow("202", "标准双人", "150", "空闲", "");
        addRow("301", "商务套房", "400", "入住", "公司协议");

        m_proxy = new QSortFilterProxyModel(this);
        m_proxy->setSourceModel(m_model);
        m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        // 默认按所有列搜索的话可以重写 filterAcceptsRow（此处简单按所有列匹配）
        m_proxy->setFilterKeyColumn(-1);

        m_table->setModel(m_proxy);
        m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_table->verticalHeader()->setVisible(false);

        v->addWidget(m_table);

        // connections
        connect(m_search, &QLineEdit::textChanged, this, &RoomsWidget::onSearch);
        connect(m_filter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RoomsWidget::onFilter);
        connect(refresh, &QPushButton::clicked, this, &RoomsWidget::onRefresh);
    }

private slots:
    void onSearch(const QString &text) {
        // 跨列搜索：构建正则并让 proxy 在所有列匹配
        QRegularExpression re(text, QRegularExpression::CaseInsensitiveOption);
        m_proxy->setFilterRegularExpression(re);
    }

    void onFilter(int idx) {
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

    void onRefresh() {
        // 简单刷新示例 — 真正项目中这里从 DB 或 API 重新加载
        m_model->removeRows(0, m_model->rowCount());
        addRow("101", "单人房", "100", "空闲", "靠窗");
        addRow("102", "标准双人", "150", "入住", "VIP");
        addRow("201", "套房", "300", "维修", "空调");
        addRow("202", "标准双人", "150", "空闲", "");
        addRow("301", "商务套房", "400", "入住", "公司协议");
    }

private:
    void addRow(const QString &r, const QString &t, const QString &p, const QString &s, const QString &note) {
        QList<QStandardItem*> row;
        row << new QStandardItem(r)
            << new QStandardItem(t)
            << new QStandardItem(p)
            << new QStandardItem(s)
            << new QStandardItem(note);
        m_model->appendRow(row);
    }

    QLineEdit *m_search;
    QComboBox *m_filter;
    QTableView *m_table;
    QStandardItemModel *m_model;
    QSortFilterProxyModel *m_proxy;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        auto *tabs = new QTabWidget;
        tabs->setTabsClosable(false);
        tabs->setMovable(false);
        tabs->setTabPosition(QTabWidget::North);

        // 首页
        QWidget *home = new QWidget;
        {
            auto *v = new QVBoxLayout(home);
            auto *title = new QLabel("<h2>欢迎使用酒店管理系统</h2>");
            v->addWidget(title);
            v->addWidget(new QLabel("概览：今日入住 X 间，空闲 Y 间。"));
            v->addStretch();
        }

        // 房间
        RoomsWidget *rooms = new RoomsWidget;

        // 预订（示例表单）
        QWidget *booking = new QWidget;
        {
            auto *form = new QFormLayout(booking);
            form->addRow("客户姓名:", new QLineEdit);
            form->addRow("入住日期:", new QLineEdit);
            form->addRow("退房日期:", new QLineEdit);
            form->addRow("房型:", new QComboBox);
            form->addRow("备注:", new QTextEdit);
            form->addRow(new QLabel(""), new QPushButton("提交预订"));
        }

        // 订单（简略）
        QWidget *orders = new QWidget;
        {
            auto *v = new QVBoxLayout(orders);
            v->addWidget(new QLabel("订单列表（示例）"));
            v->addStretch();
        }

        // 设置
        QWidget *settings = new QWidget;
        {
            auto *v = new QVBoxLayout(settings);
            v->addWidget(new QLabel("系统设置"));
            v->addStretch();
        }

        tabs->addTab(home, "首页");
        tabs->addTab(rooms, "房间");
        tabs->addTab(booking, "预订");
        tabs->addTab(orders, "订单");
        tabs->addTab(settings, "设置");

        setCentralWidget(tabs);
        setWindowTitle("酒店管理系统（示例）");
        resize(900, 600);

        // 可选：监听 tab 切换事件
        connect(tabs, &QTabWidget::currentChanged, this, [](int index){
            // index 从 0 开始
            qDebug("切换到 tab index: %d", index);
        });
    }
};

#include "auto_test.moc"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}












