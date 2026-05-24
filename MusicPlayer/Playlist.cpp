#include "stdafx.h"
#include "Playlist.h"
#include <algorithm>

void Playlist::setSongList(std::vector<SongListInfo>& list)
{
	ui.songList->clear();
	_song_list = list;
	for (auto& it : _song_list)
	{
		SongInfo* info = new SongInfo(this);
		info->setInfo(it);
		QListWidgetItem* item = new QListWidgetItem();
		item->setSizeHint(info->sizeHint());
		ui.songList->addItem(item);
		ui.songList->setItemWidget(item, info);
	}
}

void Playlist::setPlaylist(const PlaylistInfo& info)
{
	_playlist_info = info;
	QPixmap image = info.image;
	image.scaled(QSize(130, 130));
	ui.image->setPixmap(image);
	ui.playlistName->setText(info.name);
	ui.userName->setText(info.user_name);
	ui.created_at->setText(QString::number(info.created_at.year()) + QString("-") + QString::number(info.created_at.month()) + QString("-") + QString::number(info.created_at.day()));
}

Playlist::Playlist(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.songList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	ui.songList->verticalScrollBar()->setSingleStep(10);
}

Playlist::~Playlist()
{}

