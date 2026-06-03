#pragma once

#include <QWidget>
#include "ui_VolumePopup.h"

class VolumeButton;

class VolumePopup : public QWidget
{
	Q_OBJECT

public:
	VolumePopup(QWidget *parent = nullptr,VolumeButton* button = nullptr);
	~VolumePopup();
	void setVolume(float num);
	int getWidth();
	int getHeight();
	bool isMouseInside();
protected:
	void leaveEvent(QEvent* event) override;

private:
	Ui::VolumePopupClass ui;
	VolumeButton* _button;
};
