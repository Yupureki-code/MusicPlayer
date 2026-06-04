#include "stdafx.h"
#include "MusicPlayer.h"
#include <QtWidgets/QApplication>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    // 高DPI支持
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);

    // 设置默认字体 - 使用系统中渲染效果最好的中文字体
    QFont defaultFont;
    defaultFont.setPointSize(10);
    defaultFont.setHintingPreference(QFont::PreferFullHinting);     // 完整字体提示
    defaultFont.setStyleStrategy(QFont::PreferAntialias);          // 优先抗锯齿
    app.setFont(defaultFont);

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
