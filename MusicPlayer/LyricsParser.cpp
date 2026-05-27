#include "stdafx.h"
#include "LyricsParser.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

LyricsParser::LyricsParser()
{
}

LyricsParser::~LyricsParser()
{
}

bool LyricsParser::loadFromFile(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	QTextStream in(&file);
	in.setEncoding(QStringConverter::Utf8);
	QString content = in.readAll();
	file.close();

	return loadFromString(content);
}

bool LyricsParser::loadFromString(const QString& content)
{
	_lines.clear();

	static QRegularExpression lineRegex("\\[(\\d{2}):(\\d{2})\\.?(\\d{0,3})\\](.*)");
	QStringList lines = content.split('\n', Qt::SkipEmptyParts);

	for (const QString& line : lines)
	{
		QRegularExpressionMatch match = lineRegex.match(line.trimmed());
		if (match.hasMatch())
		{
			qint64 ms = parseTimestamp(match.captured(1) + ":" + match.captured(2) + "." + match.captured(3));
			QString text = match.captured(4).trimmed();
			if (!text.isEmpty())
			{
				_lines.append(LyricsLine(ms, text));
			}
		}
	}

	return !_lines.isEmpty();
}

int LyricsParser::findCurrentLine(qint64 positionMs) const
{
	if (_lines.isEmpty())
	{
		return -1;
	}

	int result = -1;
	for (int i = 0; i < _lines.size(); ++i)
	{
		if (_lines[i].timestampMs <= positionMs)
		{
			result = i;
		}
		else
		{
			break;
		}
	}

	return result;
}

QString LyricsParser::getLineText(int index) const
{
	if (index >= 0 && index < _lines.size())
	{
		return _lines[index].text;
	}
	return QString();
}

qint64 LyricsParser::getLineTimestamp(int index) const
{
	if (index >= 0 && index < _lines.size())
	{
		return _lines[index].timestampMs;
	}
	return 0;
}

int LyricsParser::lineCount() const
{
	return _lines.size();
}

void LyricsParser::clear()
{
	_lines.clear();
}

qint64 LyricsParser::parseTimestamp(const QString& timestampStr)
{
	QStringList parts = timestampStr.split(':');
	if (parts.size() < 2)
	{
		return 0;
	}

	bool ok1 = false;
	bool ok2 = false;
	int minutes = parts[0].toInt(&ok1);
	QString secPart = parts[1];
	int seconds = 0;
	int milliseconds = 0;

	int dotIndex = secPart.indexOf('.');
	if (dotIndex >= 0)
	{
		seconds = secPart.left(dotIndex).toInt(&ok2);
		QString msStr = secPart.mid(dotIndex + 1);
		if (msStr.size() == 1)
		{
			msStr += "00";
		}
		else if (msStr.size() == 2)
		{
			msStr += "0";
		}
		milliseconds = msStr.left(3).toInt();
	}
	else
	{
		seconds = secPart.toInt(&ok2);
	}

	if (!ok1 || !ok2)
	{
		return 0;
	}

	return static_cast<qint64>(minutes) * 60000 + static_cast<qint64>(seconds) * 1000 + milliseconds;
}
