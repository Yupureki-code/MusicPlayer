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
protected:
    void enterEvent(QEnterEvent* event);
	void leaveEvent(QEvent* event);
private:
	VolumePopup* _pop;
};

