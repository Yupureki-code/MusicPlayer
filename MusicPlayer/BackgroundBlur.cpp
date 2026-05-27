#include "stdafx.h"
#include "BackgroundBlur.h"
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>

QPixmap BackgroundBlur::captureAndBlur(QWidget* source, int blurRadius)
{
	if (!source)
	{
		return QPixmap();
	}

	QPixmap screenshot = source->grab();
	QPixmap blurred = applyBlur(screenshot, blurRadius);
	return addOverlay(blurred, QColor(0, 0, 0, 100));
}

QPixmap BackgroundBlur::applyBlur(const QPixmap& source, int blurRadius)
{
	if (source.isNull())
	{
		return source;
	}

	QGraphicsScene scene;
	QGraphicsPixmapItem* item = scene.addPixmap(source);

	QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect();
	blurEffect->setBlurRadius(blurRadius);
	item->setGraphicsEffect(blurEffect);

	QPixmap result(source.size());
	result.fill(Qt::transparent);
	QPainter painter(&result);
	scene.render(&painter, QRectF(), QRectF(0, 0, source.width(), source.height()));
	painter.end();

	return result;
}

QPixmap BackgroundBlur::addOverlay(const QPixmap& source, const QColor& overlayColor)
{
	if (source.isNull())
	{
		return source;
	}

	QPixmap result(source.size());
	result.fill(Qt::transparent);
	QPainter painter(&result);
	painter.drawPixmap(0, 0, source);
	painter.fillRect(source.rect(), overlayColor);
	painter.end();

	return result;
}
