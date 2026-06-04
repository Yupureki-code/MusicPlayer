#pragma once

#include <QWidget>
#include "ui_SongSetting.h"
#include "AudioEngine.h"

class SongSetting : public QWidget
{
	Q_OBJECT

public:
	SongSetting(QWidget *parent = nullptr);
	~SongSetting();
public slots:
	void valueChanged(int num);
	void sliderReleased();
	void valueReset();
private:
	Ui::SongSettingClass ui;
};

