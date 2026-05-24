#include "stdafx.h"
#include "VolumeButton.h"

VolumeButton::VolumeButton(QWidget *parent)
    : QPushButton(parent)
    , _pop(new VolumePopup(nullptr))  // 无父窗口，独立的顶层 popup
{
    _pop->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    _pop->setAttribute(Qt::WA_ShowWithoutActivating);

}

VolumeButton::~VolumeButton()
{}

void VolumeButton::enterEvent(QEnterEvent *event)
{
    // 弹窗位置：按钮上方，需要先确定弹窗尺寸
    _pop->adjustSize();
    QPoint globalPos = mapToGlobal(QPoint(0, 0));
    _pop->move(globalPos.x(), globalPos.y() - _pop->height() - 4);
    _pop->setVolume(audio->getVolume());
    _pop->show();
    QPushButton::enterEvent(event);
}

void VolumeButton::leaveEvent(QEvent* event)
{
    // 延迟隐藏：避免鼠标从按钮进入弹窗时立刻关闭
    QPoint cursorPos = QCursor::pos();
    QRect popRect = _pop->geometry();
    if (!popRect.contains(cursorPos))
        _pop->hide();
    QPushButton::leaveEvent(event);
}
