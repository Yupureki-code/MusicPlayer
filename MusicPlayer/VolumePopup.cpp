#include "stdafx.h"
#include "VolumePopup.h"
#include "AudioEngine.h"
#include <QCursor>

VolumePopup::VolumePopup(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setMouseTracking(true);
    hide();

    connect(ui.slider, &QSlider::valueChanged, this, [this](int value) {
        float num = value / 100.0f;
        audio->setVolume(num);
        });
}

void VolumePopup::leaveEvent(QEvent* event)
{
    QPoint cursorPos = QCursor::pos();
    QRect myRect = geometry();
    if (!myRect.contains(cursorPos))
        hide();
    QWidget::leaveEvent(event);
}

VolumePopup::~VolumePopup()
{}

void VolumePopup::setVolume(float num)
{
    ui.slider->setValue(num * 100);
}
