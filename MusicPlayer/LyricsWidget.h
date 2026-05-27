#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include "LyricsParser.h"

class LyricsWidget : public QWidget
{
	Q_OBJECT
public:
	LyricsWidget(QWidget *parent = nullptr);
	~LyricsWidget();

	void loadLyrics(const QString& filePath);
	void updatePosition(qint64 positionMs);
	void clear();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	void createLyricsLabels();
	void scrollToLine(int lineIndex);

	QScrollArea* _scrollArea = nullptr;
	QWidget* _contentWidget = nullptr;
	QVBoxLayout* _contentLayout = nullptr;
	QVector<QLabel*> _lyricsLabels;
	LyricsParser _parser;
	int _currentLine = -1;
	QPropertyAnimation* _scrollAnimation = nullptr;
};
