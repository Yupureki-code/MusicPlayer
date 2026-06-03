#pragma once

#include <QPushButton>
#include <VolumePopup.h>
#include "AudioEngine.h"

class VolumeButton  : public QPushButton
{
	Q_OBJECT

public:
	VolumeButton(QWidget *parent);
	~VolumeButton();
	void setVolume(float volume);
	bool isMouseInside();
public slots:
	void on_VolumeButton_clicked();
protected:
    void enterEvent(QEnterEvent* event);
	void leaveEvent(QEvent* event);
private:
	VolumePopup* _pop;
	bool _is_mute = false;
	float _prev_volume = 0;
};

