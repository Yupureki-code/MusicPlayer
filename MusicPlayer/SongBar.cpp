#include "stdafx.h"
#include "SongBar.h"

SongBar::SongBar(QWidget *parent)
	: QSlider(parent)
{
	_animation = new QPropertyAnimation(this, "progress", this);
	_animation->setDuration(200);
	_animation->setEasingCurve(QEasingCurve::OutCubic);
}

SongBar::~SongBar()
{}

void SongBar::setProgress(qreal p)
{
	_progress = p;
	updateStyleSheet();
}

void SongBar::enterEvent(QEnterEvent* event)
{
	_animation->stop();
	_animation->setStartValue(_progress);
	_animation->setEndValue(1.0);
	_animation->start();
	QSlider::enterEvent(event);
}

void SongBar::leaveEvent(QEvent* event)
{
	_animation->stop();
	_animation->setStartValue(_progress);
	_animation->setEndValue(0.0);
	_animation->start();
	QSlider::leaveEvent(event);
}

void SongBar::updateStyleSheet()
{
	// 线性插值计算当前值
	// 正常状态: trackHeight=4, handleSize=8, handleRadius=4
	// 悬停状态: trackHeight=6, handleSize=14, handleRadius=7
	int trackHeight = static_cast<int>(4 + _progress * 2);      // 4 -> 6
	int handleSize = static_cast<int>(8 + _progress * 6);       // 8 -> 14
	int handleRadius = static_cast<int>(4 + _progress * 3);     // 4 -> 7
	int handleMargin = static_cast<int>(-2 - _progress * 2);    // -2 -> -4

	QString qss = QString(R"(
		QSlider::groove:horizontal {
			border: none;
			height: %1px;
			background: rgba(255, 255, 255, 50);
			border-radius: %2px;
		}
		QSlider::sub-page:horizontal {
			background: qlineargradient(
				x1:0, y1:0, x2:1, y2:0,
				stop:0 #4FC3F7,
				stop:0.5 #AB47BC,
				stop:1 #FF6B8A
			);
			border-radius: %2px;
		}
		QSlider::add-page:horizontal {
			background: rgba(255, 255, 255, 50);
			border-radius: %2px;
		}
		QSlider::handle:horizontal {
			background: #FF6B8A;
			border: none;
			width: %3px;
			height: %3px;
			margin: %4px 0;
			border-radius: %5px;
		}
	)").arg(trackHeight).arg(trackHeight / 2).arg(handleSize).arg(handleMargin).arg(handleRadius);

	setStyleSheet(qss);
}
