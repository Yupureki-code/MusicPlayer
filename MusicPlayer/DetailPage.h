#pragma once

#include <QWidget>
#include "ui_DetailPage.h"
#include "AudioEngine.h"

enum class MouseStyle
{
	OutsideWindow,
	InsideWindow,
	BottomArea
};

class DetailPage : public QWidget
{
	Q_OBJECT
public:
	DetailPage(QWidget *parent = nullptr);
	~DetailPage();

	void setCoverImage(const QPixmap& image);
	void setSongInfo(const QString& songName, const QString& artist, const QString& album);
	void setBackground(const QPixmap& background);
	void updateMouseStyle(MouseStyle style);

protected:
	void enterEvent(QEnterEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

signals:
	void backClicked();
	void playClicked();
	void lastSongClicked();
	void nextSongClicked();
	void fullscreenClicked();
	void likeClicked();
	void favoriteClicked();
	void downloadClicked();
	void shareClicked();
	void settingClicked();
	void lyricsClicked();
	void volumeClicked();
	void songListClicked();

private slots:
	void on_backBtn_clicked();
	void on_playBtn_clicked();
	void on_lastSongBtn_clicked();
	void on_nextSongBtn_clicked();
	void on_fullscreenBtn_clicked();

private:
	Ui::DetailPageClass ui;
	MouseStyle _currentStyle = MouseStyle::OutsideWindow;
	bool _is_mouse_inside = false;
	bool _is_mouse_in_bottom = false;
	QPixmap _background;
};
