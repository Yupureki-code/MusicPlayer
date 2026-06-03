#pragma once

#include <QWidget>
#include "ui_SongCover.h"

class SongCover : public QWidget
{
	Q_OBJECT

public:
	SongCover(QWidget *parent = nullptr);
	~SongCover();
	void setPixmap(const QPixmap& image);
protected:
	void enterEvent(QEnterEvent* event);
	void leaveEvent(QEvent* event);
private:
	Ui::SongCoverClass ui;
	QPixmap _src;
	QPixmap _dark;
	QLabel* _expand;
};

