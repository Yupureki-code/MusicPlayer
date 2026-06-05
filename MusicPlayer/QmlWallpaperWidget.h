#pragma once

#include <QWidget>
#include <QQuickWidget>
#include <QQuickItem>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QPushButton>

class QmlWallpaperWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QmlWallpaperWidget(QWidget *parent = nullptr);
    ~QmlWallpaperWidget();

    void setVideoSource(const QString &path);
    void setBlurRadius(qreal radius);
    void setBrightness(qreal value);
    void setZoomFactor(qreal factor);

    int fps() const;
    qreal frameTime() const;

private slots:
    void onQmlLoaded();

private:
    QQuickWidget *m_quickWidget = nullptr;
};
