#pragma once

#include <QPushButton>
#include "SongSetting.h"

class SongSettingBT  : public QPushButton
{
	Q_OBJECT

public:
	SongSettingBT(QWidget *parent);
	~SongSettingBT();
public slots:
	void Clicked();
private:
	SongSetting* _setting;
	bool _is_showing = false;
};

