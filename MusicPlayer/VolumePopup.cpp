#include "stdafx.h"
#include "VolumePopup.h"
#include "AudioEngine.h"
#include <QCursor>
#include <QTimer>
#include "VolumeButton.h"

VolumePopup::VolumePopup(QWidget *parent, VolumeButton* button)
    : QWidget(parent),_button(button)
{
    ui.setupUi(this);
    setMouseTracking(true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    hide();

    connect(ui.slider, &QSlider::valueChanged, this, [this](int value) {
        float num = value / 100.0f;
        audio->setVolume(num);
        });
}

void VolumePopup::leaveEvent(QEvent* event)
{
    qDebug() << "VolumePopup::leaveEvent called";
    // 使用 QTimer::singleShot 延迟检查，避免鼠标在按钮和弹窗之间移动时闪烁
    QTimer::singleShot(100, this, [this]() {
        QPoint cursorPos = QCursor::pos();
        QRect popRect = geometry();
        QRect buttonRect = QRect(_button->mapToGlobal(QPoint(0, 0)), _button->size());
        // 只有当鼠标不在按钮和弹窗内时才隐藏
        if (!popRect.contains(cursorPos) && !buttonRect.contains(cursorPos))
        {
            qDebug() << "VolumePopup: hiding popup";
            hide();
        }
    });
    QWidget::leaveEvent(event);
}

VolumePopup::~VolumePopup()
{}

void VolumePopup::setVolume(float num)
{
    ui.slider->setValue(num * 100);
    _button->setVolume(num);
}

int VolumePopup::getWidth()
{
    return width();
}

int VolumePopup::getHeight()
{
    return height();
}

bool VolumePopup::isMouseInside()
{
    // 将控件的矩形区域映射到全局坐标
    QRect widgetGlobalRect = QRect(mapToGlobal(QPoint(0, 0)), size());
    // 获取当前鼠标全局位置并判断
    return widgetGlobalRect.contains(QCursor::pos());
}
