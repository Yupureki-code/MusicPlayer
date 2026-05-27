#pragma once

#include <QPixmap>
#include <QWidget>
#include <QColor>

class BackgroundBlur
{
public:
	static QPixmap captureAndBlur(QWidget* source, int blurRadius = 20);
	static QPixmap applyBlur(const QPixmap& source, int blurRadius);
	static QPixmap addOverlay(const QPixmap& source, const QColor& overlayColor);
};
