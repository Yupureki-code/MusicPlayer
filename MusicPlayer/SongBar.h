#pragma once

#include <QSlider>
#include <QPropertyAnimation>

class SongBar : public QSlider
{
	Q_OBJECT
	Q_PROPERTY(qreal progress READ progress WRITE setProgress)

public:
	explicit SongBar(QWidget *parent = nullptr);
	~SongBar();

	qreal progress() const { return _progress; }
	void setProgress(qreal p);

protected:
	void enterEvent(QEnterEvent* event) override;
	void leaveEvent(QEvent* event) override;

private:
	void updateStyleSheet();

	qreal _progress = 0.0;  // 0.0 = 正常状态, 1.0 = 悬停状态
	QPropertyAnimation* _animation = nullptr;
};
