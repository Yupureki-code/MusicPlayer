#include "stdafx.h"
#include "DetailPage.h"
#include <QPainter>

DetailPage::DetailPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	updateMouseStyle(MouseStyle::OutsideWindow);
}

DetailPage::~DetailPage()
{
}

void DetailPage::setCoverImage(const QPixmap& image)
{
	ui.coverLabel->setPixmap(image.scaled(ui.coverLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void DetailPage::setSongInfo(const QString& songName, const QString& artist, const QString& album)
{
	ui.songNameLabel->setText(songName);
	ui.artistLabel->setText(artist);
	ui.albumLabel->setText(album);
}

void DetailPage::setBackground(const QPixmap& background)
{
	_background = background;
	update();
}

void DetailPage::updateMouseStyle(MouseStyle style)
{
	_currentStyle = style;

	switch (style)
	{
	case MouseStyle::OutsideWindow:
		ui.fullscreenBtn->hide();
		ui.lastSongBtn->hide();
		ui.nextSongBtn->hide();
		break;

	case MouseStyle::InsideWindow:
		ui.fullscreenBtn->show();
		ui.lastSongBtn->show();
		ui.nextSongBtn->show();
		break;

	case MouseStyle::BottomArea:
		ui.fullscreenBtn->show();
		ui.lastSongBtn->show();
		ui.nextSongBtn->show();
		break;
	}
}

void DetailPage::enterEvent(QEnterEvent* event)
{
	_is_mouse_inside = true;
	updateMouseStyle(MouseStyle::InsideWindow);
}

void DetailPage::leaveEvent(QEvent* event)
{
	_is_mouse_inside = false;
	_is_mouse_in_bottom = false;
	updateMouseStyle(MouseStyle::OutsideWindow);
}

void DetailPage::mouseMoveEvent(QMouseEvent* event)
{
	int bottomThreshold = height() - 100;
	bool inBottom = event->pos().y() > bottomThreshold;

	if (inBottom != _is_mouse_in_bottom)
	{
		_is_mouse_in_bottom = inBottom;
		updateMouseStyle(inBottom ? MouseStyle::BottomArea : MouseStyle::InsideWindow);
	}
}

void DetailPage::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	if (!_background.isNull())
	{
		painter.drawPixmap(0, 0, _background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	}
	else
	{
		painter.fillRect(rect(), QColor(30, 30, 30));
	}
	QWidget::paintEvent(event);
}

void DetailPage::on_backBtn_clicked()
{
	emit backClicked();
}

void DetailPage::on_playBtn_clicked()
{
	emit playClicked();
}

void DetailPage::on_lastSongBtn_clicked()
{
	emit lastSongClicked();
}

void DetailPage::on_nextSongBtn_clicked()
{
	emit nextSongClicked();
}

void DetailPage::on_fullscreenBtn_clicked()
{
	emit fullscreenClicked();
}
