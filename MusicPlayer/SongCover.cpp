#include "stdafx.h"
#include "SongCover.h"
#include <iostream>

SongCover::SongCover(QWidget *parent)
	: QWidget(parent),_expand(nullptr)
{
	ui.setupUi(this);
	// 确保songImage背景透明
	ui.songImage->setStyleSheet("background: transparent;");

	// 必须在setupUi之后创建，否则会被布局覆盖
	_expand = new QLabel(this);
	QPixmap image(":/image/image/expand.png");
	QPixmap scaledImage = image.scaled(QSize(20,20), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	_expand->resize(20, 20);
	_expand->setPixmap(scaledImage);
	_expand->hide();
	_expand->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	_expand->setAttribute(Qt::WA_TranslucentBackground, true);
	_expand->setAttribute(Qt::WA_NoSystemBackground, true);
	_expand->setStyleSheet("background: transparent;");
	_expand->setAlignment(Qt::AlignCenter);

}

SongCover::~SongCover()
{}


void SongCover::setPixmap(const QPixmap& image)
{
	QPixmap scaledImage = image.scaled(QSize(width(), height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	_src = scaledImage;
	_dark = scaledImage;
	QPainter painter(&_dark);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.fillRect(_dark.rect(), QColor(0, 0, 0, 100));
	ui.songImage->setPixmap(_src);
}

void SongCover::enterEvent(QEnterEvent * event)
{
	if (_expand)
	{
		// 居中显示展开图标
		int x = (width() - _expand->width()) / 2;
		int y = (height() - _expand->height()) / 2;
		_expand->move(x, y);
		_expand->raise();  // 确保在最上层
		_expand->show();
	}
	if(!_dark.isNull())
		ui.songImage->setPixmap(_dark);
	QWidget::enterEvent(event);
}

void SongCover::leaveEvent(QEvent* event)
{
	if (_expand)
		_expand->hide();
	if(!_src.isNull())
		ui.songImage->setPixmap(_src);
	QWidget::leaveEvent(event);
}

