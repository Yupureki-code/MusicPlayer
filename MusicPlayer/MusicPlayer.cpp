#include "stdafx.h"
#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    InitUi();
}

void MusicPlayer::InitUi()
{
    // 取消Windows标题栏
    this->setWindowFlag(Qt::WindowType::FramelessWindowHint);
    // 设置窗口透明
    setAttribute(Qt::WA_TranslucentBackground,true);
    // 设置边框阴影
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(15.0); // 模糊半径，值越大阴影越模糊、扩散越远
    shadow->setOffset(5.0, 5.0); // 偏移量，控制阴影在水平和垂直方向的距离
    shadow->setColor(QColor(0, 0, 0, 150)); // 阴影颜色，最后一个参数是透明度(0-255)
    this->setGraphicsEffect(shadow);
}

void MusicPlayer::on_quit_clicked()
{
    close();
}

MusicPlayer::~MusicPlayer()
{}

