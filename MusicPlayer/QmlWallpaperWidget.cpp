#include "stdafx.h"
#include "QmlWallpaperWidget.h"
#include <QUrl>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <filesystem>

QmlWallpaperWidget::QmlWallpaperWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // 创建 QQuickWidget
    m_quickWidget = new QQuickWidget(this);
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_quickWidget->setClearColor(Qt::transparent);

    // 加载 QML - 尝试多个路径
    QString qmlPath = QCoreApplication::applicationDirPath() + "/qml/DynamicWallpaper.qml";
    QFileInfo qmlFile(qmlPath);
    
    if (!qmlFile.exists()) {
        // 尝试源码目录
        qmlPath = "E:/C C++ Files/MusicPlayer/MusicPlayer/qml/DynamicWallpaper.qml";
        qmlFile.setFile(qmlPath);
    }
    
    qDebug() << "QML path:" << qmlPath << "exists:" << qmlFile.exists();
    
    if (qmlFile.exists()) {
        m_quickWidget->setSource(QUrl::fromLocalFile(qmlPath));
    } else {
        qWarning() << "QML file not found!";
    }

    if (m_quickWidget->status() == QQuickWidget::Error) {
        qWarning() << "QML load error:" << m_quickWidget->errors();
    } else {
        onQmlLoaded();
    }

    layout->addWidget(m_quickWidget);
}

QmlWallpaperWidget::~QmlWallpaperWidget()
{
}

void QmlWallpaperWidget::setVideoSource(const QString &path)
{
    qDebug() << "Setting video source:" << path;
    if (m_quickWidget->rootObject()) {
        // 使用 file:/// 格式的 URL
        QUrl url = QUrl::fromLocalFile(path);
        qDebug() << "URL:" << url.toString();
        m_quickWidget->rootObject()->setProperty("videoSource", url);
    } else {
        qWarning() << "QML root object is null!";
    }
}

void QmlWallpaperWidget::setBlurRadius(qreal radius)
{
    if (m_quickWidget->rootObject()) {
        m_quickWidget->rootObject()->setProperty("blurRadius", radius);
    }
}

void QmlWallpaperWidget::setBrightness(qreal value)
{
    if (m_quickWidget->rootObject()) {
        m_quickWidget->rootObject()->setProperty("brightness", value);
    }
}

void QmlWallpaperWidget::setZoomFactor(qreal factor)
{
    if (m_quickWidget->rootObject()) {
        m_quickWidget->rootObject()->setProperty("zoomFactor", factor);
    }
}

void QmlWallpaperWidget::onQmlLoaded()
{
    qDebug() << "QML loaded successfully";
    qDebug() << "Root object:" << m_quickWidget->rootObject();
}

int QmlWallpaperWidget::fps() const
{
    if (m_quickWidget->rootObject()) {
        return m_quickWidget->rootObject()->property("fps").toInt();
    }
    return 0;
}

qreal QmlWallpaperWidget::frameTime() const
{
    if (m_quickWidget->rootObject()) {
        return m_quickWidget->rootObject()->property("frameTime").toDouble();
    }
    return 0;
}
