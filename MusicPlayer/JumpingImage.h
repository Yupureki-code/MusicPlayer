#pragma once

#include <QWidget>
#include "ui_JumpingImage.h"
#include <qpropertyanimation.h>
class MusicPlayer;

class JumpingImage : public QWidget
{
	const int default_offset = 20;
	Q_OBJECT
	Q_PROPERTY(int verticalOffset READ verticalOffset WRITE setVerticalOffset)
private:
	void initAnimation();
public:
	JumpingImage(QWidget *parent = nullptr);
	~JumpingImage();
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEnterEvent*) override;
	void leaveEvent(QEvent*) override;
	void setImage(const QPixmap& image);
	int verticalOffset()const;
	void setVerticalOffset(int offset);
private:
	Ui::JumpingImageClass ui;
	QPixmap _image;
	int _current_offset;
	const int _max_offset;
	QPropertyAnimation* _animation;
};

