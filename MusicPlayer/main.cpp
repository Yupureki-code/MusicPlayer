#include "stdafx.h"
#include "MusicPlayer.h"
#include <QtWidgets/QApplication>
#include <QFont>
#include <QQuickWindow>

int main(int argc, char *argv[])
{
    // 高DPI支持
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);

    // 设置默认字体
    QFont defaultFont;
    defaultFont.setPointSize(10);
    defaultFont.setHintingPreference(QFont::PreferFullHinting);
    defaultFont.setStyleStrategy(QFont::PreferAntialias);
    app.setFont(defaultFont);

    // 打印当前渲染后端信息，用于调试
    {
        QQuickWindow tempWindow;
        qDebug() << "Graphics API:" << tempWindow.rendererInterface()->graphicsApi();
    }

    MusicPlayer window;
    window.show();
    return app.exec();
}
