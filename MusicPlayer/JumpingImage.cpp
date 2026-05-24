#include "stdafx.h"
#include "JumpingImage.h"
#include "MusicPlayer.h"

void JumpingImage::initAnimation()
{
	_animation = new QPropertyAnimation(this, "verticalOffset");
	_animation->setDuration(300);
	_animation->setEasingCurve(QEasingCurve::OutQuad);
}

JumpingImage::JumpingImage(QWidget* parent)
	: QWidget(parent),_max_offset(default_offset),_current_offset(default_offset)
{
	ui.setupUi(this);
	QMetaObject::invokeMethod(this, &JumpingImage::initAnimation, Qt::QueuedConnection);
}

JumpingImage::~JumpingImage()
{}

void JumpingImage::paintEvent(QPaintEvent * event)
{
	qDebug() << "paintEvent called:" << objectName() << width() << "X" << height();
	QPainter painter(this);
	if (_image.isNull()) {
		qDebug() << "paintEvent: _image is null, nothing to draw";
		return;
	}
	// 图片缩放至控件宽度（保持比例）
	QPixmap scaled = _image.scaledToWidth(width(), Qt::SmoothTransformation);
	int imgH = scaled.height();

	// 确保图片高度大于控件高度，否则没有切割效果
	if (imgH <= height()) {
		painter.drawPixmap(0, 0, scaled);
		return;
	}

	// 源矩形：从图片的 (0, 0) 位置开始，取控件大小
	QRect sourceRect(0, 0, width(), height());
	// 目标矩形：偏移量_current_offset以下的控件区域
	QRect targetRect(0, _current_offset, width(), height());

	painter.drawPixmap(targetRect, scaled, sourceRect);
}

void JumpingImage::enterEvent(QEnterEvent*)
{
	_animation->stop();
	_animation->setStartValue(_current_offset);
	_animation->setEndValue(0);
	_animation->start();
}

void JumpingImage::leaveEvent(QEvent*)
{
	_animation->stop();
	_animation->setStartValue(_current_offset);
	_animation->setEndValue(_max_offset);
	_animation->start();
}

void JumpingImage::setImage(const QPixmap& image)
{
	_image = image;
	update();  // 触发重绘，确保图片立即显示
}

int JumpingImage::verticalOffset() const
{
	return _current_offset;
}

void JumpingImage::setVerticalOffset(int offset)
{
	_current_offset = offset;
	update();
}

