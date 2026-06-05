#include "stdafx.h"
#include "PlayBox.h"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

PlayBox::PlayBox(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setStyleSheet("");
	setAttribute(Qt::WA_TranslucentBackground, true);
	setAttribute(Qt::WA_NoSystemBackground, true);
	setMouseTracking(true);
}

PlayBox::~PlayBox()
{}

void PlayBox::setBackground(const QPixmap& bg)
{
	m_background = bg;
	m_blurredDirty = true;
	update();
}

void PlayBox::invalidateBackground()
{
	m_blurredDirty = true;
	m_blurredBg = QPixmap();
	update();
}

void PlayBox::updateBlurredBackground()
{
	if (!m_blurredDirty)
		return;

	if (m_background.isNull() || width() <= 0 || height() <= 0)
		return;

	// 从背景图中截取对应PlayBox位置的区域
	// PlayBox位于窗口底部，需要计算它在背景图中的相对位置
	QWidget* topLevel = window();
	if (!topLevel)
		return;

	// 计算playBox在顶层窗口中的位置比例
	QPoint posInTop = mapTo(topLevel, QPoint(0, 0));
	double xRatio = static_cast<double>(posInTop.x()) / topLevel->width();
	double yRatio = static_cast<double>(posInTop.y()) / topLevel->height();
	double wRatio = static_cast<double>(width()) / topLevel->width();
	double hRatio = static_cast<double>(height()) / topLevel->height();

	// 在背景图中截取对应区域
	int bgX = static_cast<int>(xRatio * m_background.width());
	int bgY = static_cast<int>(yRatio * m_background.height());
	int bgW = static_cast<int>(wRatio * m_background.width());
	int bgH = static_cast<int>(hRatio * m_background.height());

	// 边界检查
	bgX = qMax(0, bgX);
	bgY = qMax(0, bgY);
	bgW = qMin(bgW, m_background.width() - bgX);
	bgH = qMin(bgH, m_background.height() - bgY);

	if (bgW <= 0 || bgH <= 0)
		return;

	QPixmap fragment = m_background.copy(bgX, bgY, bgW, bgH);

	// 拉伸到PlayBox的实际大小
	QPixmap scaled = fragment.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	// 应用高斯模糊
	m_blurredBg = applyBlur(scaled, m_blurRadius);
	m_blurredDirty = false;
}

QPixmap PlayBox::applyBlur(const QPixmap& src, int radius)
{
	if (radius <= 0 || src.isNull()) return src;

	QGraphicsBlurEffect* effect = new QGraphicsBlurEffect;
	effect->setBlurRadius(radius);

	QGraphicsPixmapItem* item = new QGraphicsPixmapItem(src);
	item->setGraphicsEffect(effect);

	QGraphicsScene scene;
	scene.addItem(item);

	QPixmap result(src.size());
	result.fill(Qt::transparent);
	QPainter p(&result);
	scene.render(&p, QRectF(), QRectF(0, 0, src.width(), src.height()));
	p.end();

	return result;
}

void PlayBox::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	updateBlurredBackground();

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// 圆角裁剪
	QPainterPath path;
	int radius = 12;
	path.addRoundedRect(rect(), radius, radius);
	painter.setClipPath(path);

	// 绘制模糊背景
	if (!m_blurredBg.isNull()) {
		painter.drawPixmap(0, 0, m_blurredBg);
		// 半透明白色覆盖，增强磨砂感
		painter.fillRect(rect(), QColor(255, 255, 255, 40));
	} else {
		// 降级：半透明背景
		painter.fillRect(rect(), QColor(100, 100, 100, 120));
	}

	// 圆角边框
	painter.setClipping(false);
	painter.setPen(QPen(QColor(255, 255, 255, 60), 1));
	painter.setBrush(Qt::NoBrush);
	painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), radius, radius);
}

void PlayBox::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	m_blurredDirty = true;
	update();
}

void PlayBox::initChildStyles()
{
	for (QLabel* label : findChildren<QLabel*>()) {
		label->setStyleSheet(label->styleSheet() +
			" color: white; background: transparent;");
	}
	for (QPushButton* btn : findChildren<QPushButton*>()) {
		btn->setStyleSheet(btn->styleSheet() +
			" color: white; background: transparent; border: none;");
	}
}
