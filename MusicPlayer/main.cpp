#include "stdafx.h"
#include "MusicPlayer.h"
#include <QtWidgets/QApplication>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MusicPlayer window;
    // 创建媒体播放器 和 视频部件
    QMediaPlayer* player = new QMediaPlayer;
    QVideoWidget* videoWidget = new QVideoWidget;

    // 设置视频输出
    player->setVideoOutput(videoWidget);
    player->setSource(QUrl::fromLocalFile(":/image/image/background.mp4"));
    // 自动循环播放（Qt 6 需要手动处理循环）
    QObject::connect(player, &QMediaPlayer::mediaStatusChanged,
        [player](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::EndOfMedia) {
                player->setPosition(0);
                player->play();
            }
        });

    // 开始播放
    player->play();

    // 创建一个布局，把 QVideoWidget 放进去，让它填满整个窗口
    QVBoxLayout* mainLayout = new QVBoxLayout(&window);
    mainLayout->addWidget(videoWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    window.show();
    return app.exec();
}
