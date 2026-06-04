#ifndef DYNAMICWALLPAPER_H
#define DYNAMICWALLPAPER_H
#include <QWidget>

class QMediaPlayer;
class QVideoWidget;
class DynamicWallpaper  : public QObject
{
	Q_OBJECT

public:
    /**
     * @brief 一行代码接入：让 target 窗口拥有视频动态背景
     * @param target   你的主窗口（任意 QWidget*）
     * @param videoPath 视频路径（支持 qrc 资源路径 ":/xxx.mp4" 或本地绝对路径）
     */
    static void attach(QWidget* target, const QString& videoPath)
    {
        // 只创建一次，防重复调用
        if (target->property("_dynWallAttached").toBool()) return;
        target->setProperty("_dynWallAttached", true);
        auto* wall = new DynamicWallpaper(target, videoPath);
        Q_UNUSED(wall);  // 生命周期交给 target 的父子关系自动管理
    }
private:
    explicit DynamicWallpaper(QWidget* target, const QString& videoPath);
    bool eventFilter(QObject* obj, QEvent* event) override;
    QMediaPlayer* m_player = nullptr;
    QVideoWidget* m_video = nullptr;
    QWidget* m_target = nullptr;
};
#endif
