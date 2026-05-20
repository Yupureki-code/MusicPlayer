#include "stdafx.h"
#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QWidget(parent)
{
    InitConfig();
    ui.setupUi(this);
    InitUi();
}

void MusicPlayer::InitUi()
{
    // 取消Windows标题栏
    this->setWindowFlag(Qt::WindowType::FramelessWindowHint);
    // 设置窗口透明
    setAttribute(Qt::WA_TranslucentBackground, true);

    // 移除原来给阴影预留的 9px margin，由 paintEvent 统一绘制阴影和背景
    ui.verticalLayout_3->setContentsMargins(0, 0, 0, 0);
    // background 设为透明，由 MusicPlayer::paintEvent 统一画白色背景和阴影
    ui.background->setStyleSheet("background: transparent;");

    // 设置搜索栏图标
    QLineEdit* lineEdit = ui.searchLine;
    lineEdit->setPlaceholderText("搜索...");
    QAction* searchAction = new QAction(this);
    searchAction->setIcon(QIcon(":/image/image/find.png"));
    lineEdit->addAction(searchAction, QLineEdit::LeadingPosition);
    lineEdit->setStyleSheet("QLineEdit { padding-left: 20px; }");
    connect(searchAction, &QAction::triggered, this, []() {
        qDebug() << "搜索图标被点击";
        });
    // NavigateBT 图标已在其构造函数内自初始化
    // 设置初始选中按钮（与 QStackedWidget 第 0 页对应）
    if (ui.recommend) {
        SelectNavigateBT(ui.recommend);
    } else {
        ui.mainPlayBody->setCurrentIndex(_current_page_index);
    }
    // 画出分割线
    QPainter painter;
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen; pen.setWidth(10);
    pen.setColor(QColor("#726B6F"));
    painter.setPen(pen);
    int x0 = ui.navigateBox->x() + ui.navigateBox->width();
    int y0 = ui.navigateBox->y();
    int y1 = ui.navigateBox->y() + ui.navigateBox->height();
    painter.drawLine(QLine(x0, y0, x0, y1));
}

void MusicPlayer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();
    const int blur  = 12;   // 阴影扩散半径
    const int offX  = 4;    // 水平偏移
    const int offY  = 4;    // 垂直偏移

    // ---- 绘制阴影（从外到内，alpha 递增，模拟模糊） ----
    painter.setPen(Qt::NoPen);
    for (int i = blur; i >= 1; --i) {
        int alpha = 10 + (blur - i) * 12;   // 外层淡、内层浓
        alpha = qMin(alpha, 140);
        painter.setBrush(QColor(0, 0, 0, alpha));
        QRect shadow = r.adjusted(i, i, -i + offX, -i + offY);
        painter.drawRect(shadow);
    }

    // ---- 绘制白色内容背景（占据左上角，留出右下阴影） ----
    QRect content = r.adjusted(0, 0, -offX, -offY);
    painter.setBrush(Qt::white);
    painter.drawRect(content);

    QWidget::paintEvent(event);
}

void MusicPlayer::on_minimize_clicked()
{
    showMinimized();
}

void MusicPlayer::on_maximize_clicked()
{
    if (this->isMaximized()) {
        this->showNormal();
    }
    else {
        this->showMaximized();
    }
}

void MusicPlayer::InitConfig()
{
    // 初始化图片路径
    _images["like"] = ":/image/image/like.png";
    _images["recent"] = ":/image/image/recent.png";
    _images["local"] = ":/image/image/local.png";
    _images["myPodcast"] = ":/image/image/podcast.png";
    _images["podcast"] = ":/image/image/podcast.png";
    _images["favorite"] = ":/image/image/favorite.png";

    //初始化页面下标
    _pages["recommend"] = 0;
    _pages["podcast"] = 1;
    _pages["musicHall"] = 2;
    _pages["like"] = 3;
    _pages["local"] = 4;
    _pages["recent"] = 5;
    _pages["myPodcast"] = 6;
    _pages["favorite"] = 7;
}

QString MusicPlayer::FindNavigateBTImages(NavigateBT* bt)
{
    return _images[bt->objectName()];
}

void MusicPlayer::SetCurrentPageIndex(const QString& page)
{
    _current_page_index = _pages[page];
    ui.mainPlayBody->setCurrentIndex(_current_page_index);
}

void MusicPlayer::SelectNavigateBT(NavigateBT* bt)
{
    if (!bt) return;
    // 取消旧按钮选中
    if (_currentSelectedBt && _currentSelectedBt != bt)
        _currentSelectedBt->setSelected(false);
    // 设置新按钮选中
    _currentSelectedBt = bt;
    bt->setSelected(true);
    // 同步页面
    SetCurrentPageIndex(bt->objectName());
}

void MusicPlayer::on_quit_clicked()
{
    close();
}

MusicPlayer::~MusicPlayer()
{}

