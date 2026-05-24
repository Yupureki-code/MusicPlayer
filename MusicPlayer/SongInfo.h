#pragma once

#include <QWidget>
#include "ui_SongInfo.h"

struct SongListInfo
{
	QString code;
	bool is_liked = false;
	bool is_SQ = false;
	bool is_HiRes = false;
	bool has_MV = false;
	bool is_VIP = false;
	QString title;
	QString songer;
	QString album;
	QTime time;
	SongListInfo() = default;
	SongListInfo(const QString& _code
		, const QString& _title
		, const QString& _songer
		, const QString& _album
		, const QTime& _time)
		:code(_code),title(_title),songer(_songer),album(_album),time(_time)
	{
	}
};

class SongInfo : public QWidget
{
	Q_OBJECT

public:
	SongInfo(QWidget *parent = nullptr);
	void setInfo(const SongListInfo& info);
	~SongInfo();
private:
	Ui::SongInfoClass ui;
};

