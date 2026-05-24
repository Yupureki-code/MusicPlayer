#pragma once

#include <QWidget>
#include "ui_Playlist.h"
#include"SongInfo.h"
#include "AudioEngine.h"

struct PlaylistInfo
{
	QPixmap image;
	QString name;
	QString user_name;
	QDate created_at;
	PlaylistInfo() = default;
	PlaylistInfo(const QPixmap& _image, const QString& _name, const QString& _user_name, const QDate& _created_at)
		:image(_image), name(_name), user_name(_user_name), created_at(_created_at)
	{
	}
};

class Playlist : public QWidget
{
	Q_OBJECT
public:
	Playlist(QWidget *parent = nullptr);
	void setSongList(std::vector<SongListInfo>& list);
	void setPlaylist(const PlaylistInfo& info);
	~Playlist();

private:
	PlaylistInfo _playlist_info;
	std::vector<SongListInfo> _song_list;
	Ui::PlaylistClass ui;
};

