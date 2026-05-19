#pragma once

#include <QtWidgets/QWidget>
#include "ui_MusicPlayer.h"

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();
    void InitUi();
public slots:
    void on_quit_clicked();
private:
    Ui::MusicPlayerClass ui;
};

