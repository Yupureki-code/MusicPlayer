#pragma once

#include <QtWidgets/QWidget>
#include "ui_MusicPlayer.h"
#include <unordered_map>
#include "NavigateBT.h"
#include "DynamicCard.h"
#include "AudioEngine.h"
#include "DetailPage.h"
#include "LyricsWidget.h"
#include <qdebug.h>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

class MusicPlayer : public QWidget
{
    Q_OBJECT
private:
    void InitUi();
    void InitConfig();
    void InitCarousels();
    void InitPlayLists();
    void InitPlayList();
    void InitAudio();
    void InitDetailPage();
public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();
    QString FindNavigateBTImages(NavigateBT* bt);
    QString FindImages(QString name);
    void SetCurrentPageIndex(const QString& page);
    void SelectNavigateBT(NavigateBT* bt);
    void openDetailPage();
    void closeDetailPage();
protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
signals:
    void songChanged(const SongStruct& song);
public slots:
    void on_quit_clicked();
    void on_minimize_clicked();
    void on_maximize_clicked();
    void on_play_clicked();
    void on_song_changed(const SongStruct& song);
    void on_song_playing(qint64 positionMs);
    void on_songBar_released();
    void on_songBar_pressed();
    void on_songPlay_clicked();
private:
    Ui::MusicPlayerClass ui;
    std::unordered_map<QString, QString> _images;
    std::unordered_map<QString, int> _pages;
    int _current_page_index = 0;
    bool _is_seeking;
    NavigateBT* _currentSelectedBt = nullptr;
    DetailPage* _detailPage = nullptr;
    LyricsWidget* _lyricsWidget = nullptr;
    QParallelAnimationGroup* _openAnimGroup = nullptr;
    bool _is_detail_open = false;
    bool _is_animating = false;
};

