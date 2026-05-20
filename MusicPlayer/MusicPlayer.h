#pragma once

#include <QtWidgets/QWidget>
#include "ui_MusicPlayer.h"
#include <unordered_map>
#include "NavigateBT.h"
#include <qdebug.h>

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();
    void InitUi();
    void InitConfig();
    QString FindNavigateBTImages(NavigateBT* bt);
    void SetCurrentPageIndex(const QString& page);
    void SelectNavigateBT(NavigateBT* bt);
protected:
    void paintEvent(QPaintEvent* event);

public slots:
    void on_quit_clicked();
    void on_minimize_clicked();
    void on_maximize_clicked();
private:
    Ui::MusicPlayerClass ui;
    std::unordered_map<QString, QString> _images;
    std::unordered_map<QString, int> _pages;
    int _current_page_index = 0;
    NavigateBT* _currentSelectedBt = nullptr;
};

