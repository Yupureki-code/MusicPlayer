#pragma once

#include <QWidget>

class TitleBar  : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget*parent);
	void mousePressEvent(QMouseEvent* event);
	~TitleBar();
};

