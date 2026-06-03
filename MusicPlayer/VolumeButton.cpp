#include "stdafx.h"
#include "VolumeButton.h"
#include <QTimer>

VolumeButton::VolumeButton(QWidget *parent)
    : QPushButton(parent)
    , _pop(new VolumePopup(nullptr,this))  // 无父窗口，独立的顶层 popup
{
    // 使用 Qt::Tool 而不是 Qt::Popup，避免捕获鼠标事件
    _pop->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    _pop->setAttribute(Qt::WA_ShowWithoutActivating);
    _pop->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    // 直接在构造函数中连接点击信号
    connect(this, &QPushButton::clicked, this, &VolumeButton::on_VolumeButton_clicked);
}

VolumeButton::~VolumeButton()
{}

void VolumeButton::on_VolumeButton_clicked()
{
    qDebug() << "on_VolumeButton_clicked called!";
    if (_is_mute)
    {
        setStyleSheet(QString("image: url(:/image/image/songOn.png);border:none; "));
        _is_mute = false;
        _pop->setVolume(_prev_volume);
        audio->setVolume(_prev_volume);
    }
    else
    {
        _prev_volume = audio->getVolume();
        setStyleSheet(QString("image: url(:/image/image/songOff.png);border:none; "));
        _is_mute = true;
        _pop->setVolume(0);
        audio->setVolume(0);
    }
}

void VolumeButton::enterEvent(QEnterEvent *event)
{

    // 弹窗位置：按钮上方，需要先确定弹窗尺寸
    _pop->adjustSize();
    QPoint globalPos = mapToGlobal(QPoint(0, 0));
    _pop->move(globalPos.x() - (_pop->getWidth() - width())/2, globalPos.y() - _pop->height() - 4);
    _pop->setVolume(audio->getVolume());
    _pop->show();
    QPushButton::enterEvent(event);
}

void VolumeButton::leaveEvent(QEvent* event)
{
    // 使用 QTimer::singleShot 延迟检查，避免 Qt::Popup 显示时立即触发
    QTimer::singleShot(100, this, [this]() {
        QPoint cursorPos = QCursor::pos();
        QRect buttonRect = QRect(mapToGlobal(QPoint(0, 0)), size());
        QRect popRect = _pop->geometry();
        // 只有当鼠标不在按钮和弹窗内时才隐藏
        if (!buttonRect.contains(cursorPos) && !popRect.contains(cursorPos))
        {
            qDebug() << "hiding popup from leaveEvent";
            _pop->hide();
        }
    });
    QPushButton::leaveEvent(event);
}

void VolumeButton::setVolume(float volume)
{
    if (volume == 0 && !_is_mute)
    {
        setStyleSheet(QString("image: url(:/image/image/songOff.png);border:none; "));
        _is_mute = true;
        _prev_volume = volume;
    }
    else if (volume != 0 && _is_mute)
    {
        setStyleSheet(QString("image: url(:/image/image/songOn.png);border:none; "));
        _is_mute = false;
        _prev_volume = volume;
    }
}

bool VolumeButton::isMouseInside()
{
    // 将控件的矩形区域映射到全局坐标
    QRect widgetGlobalRect = QRect(mapToGlobal(QPoint(0, 0)), size());
    // 获取当前鼠标全局位置并判断
    return widgetGlobalRect.contains(QCursor::pos());
}
