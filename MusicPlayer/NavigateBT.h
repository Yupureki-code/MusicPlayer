#pragma once

#include <QWidget>
#include "ui_NavigateBT.h"
#include <QPainter>

class MusicPlayer;

class NavigateBT : public QWidget
{
	Q_OBJECT
public:
	NavigateBT(QWidget* parent = nullptr);
	~NavigateBT();
	void setSelected(bool selected);

private:
	void resolveParent();          // 向上查找 MusicPlayer，安全赋值 _parent
	void InitIcon();
	void InitStyle();
	void InitLabelTransparent();   // 让所有子 QLabel 背景透明
	// [毛玻璃效果 - 为后续皮肤切换保留，当前未使用]
	void captureBackground();
	void paintEvent(QPaintEvent* event);
	void updateLabelStyle();
	void enterEvent(QEnterEvent* event);
	void leaveEvent(QEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	// [毛玻璃效果 - 为后续皮肤切换保留，当前未使用]
	QPixmap applyBlur(const QPixmap& src, int radius);
private:
	Ui::NavigateBTClass ui;
	int m_blurRadius = 0;       // 0: 正常, 8: 悬停, 15: 按下
	bool m_selected = false;    // 持久选中状态（对应 QStackedWidget 当前页）
	bool m_pressed = false;
	MusicPlayer* _parent = nullptr;

	// 预缓存的背景图（毛玻璃效果保留字段，当前未使用）
	QPixmap m_bgNormal;         // blur=0
	QPixmap m_bgHover;          // blur=8
	QPixmap m_bgPressed;        // blur=15
	bool m_bgCached = false;
};
