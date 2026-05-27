#include "stdafx.h"
#include "LyricsWidget.h"
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QScrollBar>

LyricsWidget::LyricsWidget(QWidget *parent)
	: QWidget(parent)
{
	_scrollArea = new QScrollArea(this);
	_scrollArea->setWidgetResizable(true);
	_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_scrollArea->setStyleSheet("background: transparent; border: none;");

	_contentWidget = new QWidget();
	_contentLayout = new QVBoxLayout(_contentWidget);
	_contentLayout->setAlignment(Qt::AlignCenter);
	_contentLayout->setSpacing(15);

	_scrollArea->setWidget(_contentWidget);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(_scrollArea);
}

LyricsWidget::~LyricsWidget()
{
}

void LyricsWidget::loadLyrics(const QString& filePath)
{
	clear();
	if (_parser.loadFromFile(filePath))
	{
		createLyricsLabels();
	}
}

void LyricsWidget::updatePosition(qint64 positionMs)
{
	int newLine = _parser.findCurrentLine(positionMs);
	if (newLine != _currentLine && newLine >= 0 && newLine < _lyricsLabels.size())
	{
		_currentLine = newLine;
		scrollToLine(_currentLine);

		for (int i = 0; i < _lyricsLabels.size(); ++i)
		{
			QLabel* label = _lyricsLabels[i];
			if (i == _currentLine)
			{
				label->setStyleSheet("color: white; font-size: 20px; font-weight: bold; background: transparent;");
			}
			else
			{
				label->setStyleSheet("color: rgba(255, 255, 255, 100); font-size: 14px; background: transparent;");
			}
		}
	}
}

void LyricsWidget::clear()
{
	_currentLine = -1;
	for (QLabel* label : _lyricsLabels)
	{
		_contentLayout->removeWidget(label);
		delete label;
	}
	_lyricsLabels.clear();
	_parser.clear();
}

void LyricsWidget::paintEvent(QPaintEvent* event)
{
	QWidget::paintEvent(event);
}

void LyricsWidget::createLyricsLabels()
{
	QSpacerItem* topSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	_contentLayout->addItem(topSpacer);

	for (int i = 0; i < _parser.lineCount(); ++i)
	{
		QLabel* label = new QLabel(_parser.getLineText(i));
		label->setAlignment(Qt::AlignCenter);
		label->setStyleSheet("color: rgba(255, 255, 255, 100); font-size: 14px; background: transparent;");
		label->setWordWrap(true);
		_contentLayout->addWidget(label);
		_lyricsLabels.append(label);
	}

	QSpacerItem* bottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	_contentLayout->addItem(bottomSpacer);
}

void LyricsWidget::scrollToLine(int lineIndex)
{
	if (lineIndex < 0 || lineIndex >= _lyricsLabels.size())
		return;

	QLabel* targetLabel = _lyricsLabels[lineIndex];
	int targetY = targetLabel->y() + targetLabel->height() / 2 - _scrollArea->height() / 2;

	if (_scrollAnimation)
	{
		_scrollAnimation->stop();
		delete _scrollAnimation;
	}

	_scrollAnimation = new QPropertyAnimation(_scrollArea->verticalScrollBar(), "value");
	_scrollAnimation->setDuration(300);
	_scrollAnimation->setStartValue(_scrollArea->verticalScrollBar()->value());
	_scrollAnimation->setEndValue(targetY);
	_scrollAnimation->setEasingCurve(QEasingCurve::OutCubic);
	_scrollAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
