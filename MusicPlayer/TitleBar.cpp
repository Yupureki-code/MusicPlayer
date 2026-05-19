#include "stdafx.h"
#include "TitleBar.h"

TitleBar::TitleBar(QWidget*parent)
	: QWidget(parent)
{}

void TitleBar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) 
    {
        if (window()->windowHandle()) 
        {
            window()->windowHandle()->startSystemMove();
        }
        event->accept();
        return;
    }
    QWidget::mousePressEvent(event);
}

TitleBar::~TitleBar()
{}

