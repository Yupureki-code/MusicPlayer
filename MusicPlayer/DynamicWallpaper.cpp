#include "stdafx.h"
#include "DynamicWallpaper.h"
#include <QResizeEvent>
#include <QFileInfo>
#include <QScreen>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QTimer>
#include <QApplication>
#include <QDebug>
DynamicWallpaper::DynamicWallpaper(QWidget* target, const QString& videoPath)
    : QObject(target)          // parent=target → target 销毁时自动释放
    , m_target(target)
{
    // ── 1. 创建视频渲染控件 ──
    m_video = new QVideoWidget(target);
    m_video->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
    // 关键！
    // KeepAspectRatioByExpanding = 视频缩放到"刚好覆盖窗口"，多余部分裁掉
    // 效果：中心显示，上下左右溢出部分不可见
// ── 2. 创建播放器（无音频） ──
    m_player = new QMediaPlayer(this);
    // 只绑定视频输出，不创建 QAudioOutput → 无声音
    m_player->setVideoOutput(m_video);
    // ── 3. 连接循环播放 ──
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
        this, [this](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::EndOfMedia) {
                m_player->setPosition(0);
                m_player->play();
            }
        });
    // ── 4. 加载视频 ──
    QUrl url = videoPath.startsWith(':')
        ? QUrl("qrc" + videoPath)
        : QUrl::fromLocalFile(QFileInfo(videoPath).absoluteFilePath());
    m_player->setSource(url);
    // ── 5. 安装事件过滤器，监听目标窗口的 resize ──
    m_target->installEventFilter(this);
    // ── 6. 初始调整尺寸并开始播放 ──
    m_video->setGeometry(m_target->rect());
    m_video->lower();           // 沉到最底层，让其他子控件浮在上面
    m_player->play();
}
bool DynamicWallpaper::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == m_target && event->type() == QEvent::Resize) {
        m_video->setGeometry(m_target->rect());
    }
    return QObject::eventFilter(obj, event);
}