#pragma once

#include <QtWidgets/QWidget>
#include "ui_MusicPlayer.h"
#include <unordered_map>
#include "NavigateBT.h"
#include "DynamicCard.h"
#include "AudioEngine.h"
#include <qdebug.h>

class MusicPlayer : public QWidget
{
    Q_OBJECT
private:
    void InitUi();
    void InitConfig();
    void InitCarousels();
    void InitPlayLists();
    void InitPlayList();
public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();
    QString FindNavigateBTImages(NavigateBT* bt);
    QString FindImages(QString name);
    void SetCurrentPageIndex(const QString& page);
    void SelectNavigateBT(NavigateBT* bt);
protected:
    void paintEvent(QPaintEvent* event);
signals:
    void songChanged(const SongStruct& song);
public slots:
    void on_quit_clicked();
    void on_minimize_clicked();
    void on_maximize_clicked();
    void on_play_clicked();
    void on_song_changed(const SongStruct& song);

private:
    Ui::MusicPlayerClass ui;
    std::unordered_map<QString, QString> _images;
    std::unordered_map<QString, int> _pages;
    int _current_page_index = 0;
    NavigateBT* _currentSelectedBt = nullptr;
};

