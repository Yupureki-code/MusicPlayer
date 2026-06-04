#include "stdafx.h"
#include "SongSettingBT.h"

SongSettingBT::SongSettingBT(QWidget *parent)
	: QPushButton(parent), _setting(new SongSetting(nullptr))
{
	// 设置为独立的顶层窗口，与 VolumePopup 相同的方式
	_setting->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	_setting->setAttribute(Qt::WA_ShowWithoutActivating);
	connect(this, &QPushButton::clicked, this, &SongSettingBT::Clicked);
}

SongSettingBT::~SongSettingBT()
{}

void SongSettingBT::Clicked()
{
	if (!_is_showing)
	{
		// 将按钮左上角转换为屏幕全局坐标
		QPoint global_pos = mapToGlobal(QPoint(0, 0));
		// 弹窗显示在按钮左上方
		int x = global_pos.x() - _setting->width();
		int y = global_pos.y() - _setting->height();
		_setting->move(QPoint(x, y));
		_setting->show();
		_is_showing = true;
	}
	else
	{
		_setting->hide();
		_is_showing = false;
	}
}