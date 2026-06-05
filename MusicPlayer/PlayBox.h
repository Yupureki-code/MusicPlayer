#pragma once

#include <QWidget>
#include <QPixmap>
#include "ui_PlayBox.h"

class PlayBox : public QWidget
{
	Q_OBJECT

public:
	explicit PlayBox(QWidget *parent = nullptr);
	~PlayBox();

	void setBackground(const QPixmap& bg);
	void invalidateBackground();
	void initChildStyles();
protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
private:
	void updateBlurredBackground();
	QPixmap applyBlur(const QPixmap& src, int radius);

	Ui::PlayBoxClass ui;
	QPixmap m_background;      // 原始背景图
	QPixmap m_blurredBg;       // 模糊后的背景图（整个PlayBox区域）
	bool m_blurredDirty = true;
	int m_blurRadius = 15;
};
