#pragma once

#include <QWidget>
#include "ui_VolumePopup.h"

class VolumePopup : public QWidget
{
	Q_OBJECT

public:
	VolumePopup(QWidget *parent = nullptr);
	~VolumePopup();
	void setVolume(float num);
protected:
	void leaveEvent(QEvent* event) override;

private:
	Ui::VolumePopupClass ui;
};
