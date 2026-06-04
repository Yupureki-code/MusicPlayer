#include "stdafx.h"
#include "SongSetting.h"

SongSetting::SongSetting(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	hide();
	ui.reset->hide();
	connect(ui.slider, &QSlider::valueChanged, this, &SongSetting::valueChanged);
	connect(ui.slider, &QSlider::sliderReleased, this, &SongSetting::sliderReleased);
	connect(ui.reset, &QPushButton::clicked, this, &SongSetting::valueReset);
}

SongSetting::~SongSetting()
{}

void SongSetting::valueChanged(int num)
{
	qreal speed = (qreal)num / 100.0;
	ui.speedValue->setText(QString("倍速/慢放：") + QString::number(speed, 'f', 2));
	ui.reset->show();
}

void SongSetting::sliderReleased()
{
	int num = ui.slider->value();
	qreal speed = (qreal)num / 100.0;
	audio->setSpeed(speed);
}

void SongSetting::valueReset()
{
	ui.slider->setValue(100);
	audio->setSpeed(1);
	ui.reset->hide();
}
