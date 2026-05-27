#pragma once

#include <QObject>
#include <QVector>
#include <QString>

struct LyricsLine
{
	qint64 timestampMs;
	QString text;
	LyricsLine() : timestampMs(0) {}
	LyricsLine(qint64 ms, const QString& t) : timestampMs(ms), text(t) {}
};

class LyricsParser
{
public:
	LyricsParser();
	~LyricsParser();

	bool loadFromFile(const QString& filePath);
	bool loadFromString(const QString& content);
	int findCurrentLine(qint64 positionMs) const;
	QString getLineText(int index) const;
	qint64 getLineTimestamp(int index) const;
	int lineCount() const;
	void clear();

private:
	static qint64 parseTimestamp(const QString& timestampStr);
	QVector<LyricsLine> _lines;
};
