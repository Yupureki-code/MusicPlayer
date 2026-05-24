#pragma once

#include <QWidget>
#include "ui_DynamicCard.h"
#include <vector>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QPropertyAnimation;
class QParallelAnimationGroup;
class QTimer;
QT_END_NAMESPACE

struct AdCard
{
	QPixmap image;
	QString title;
};

class DynamicCard : public QWidget
{
	Q_OBJECT
public:
	DynamicCard(QWidget *parent = nullptr);
	~DynamicCard();
	void setCards(const std::vector<AdCard>& cards);

protected:
	void resizeEvent(QResizeEvent* event) override;
	void enterEvent(QEnterEvent* event) override;
	void leaveEvent(QEvent* event) override;

private slots:
	void onAutoPlayTimeout();
	void onSlideFinished();
	void onJumpPhase1Finished();
	void onDotClicked();

private:
	void initSlots();
	void recalcGeometry();
	void refreshSlotPixmaps();
	void applyDotStyles();
	void layoutDots();

	void startAdjacentSlide(int direction);  // +1 right, -1 left
	void startJumpSlide(int targetIdx);

	QRect geometryForSlot(int slotIndex);

	static constexpr int SLIDE_DURATION = 400;
	static constexpr int JUMP_PHASE1_DURATION = 200;
	static constexpr int JUMP_PHASE2_DURATION = 400;
	static constexpr int AUTOPLAY_INTERVAL = 5000;
	static constexpr int DOT_SIZE = 12;

	// 共享的尺寸参数（在 recalcGeometry 中计算）
	int _cX = 0, _cY = 0, _cW = 0, _cH = 0;
	int _sX_l = 0, _sX_r = 0, _sY = 0, _sW = 0, _sH = 0;

	Ui::DynamicCardClass ui;
	std::vector<AdCard> _cards;
	int _currentIndex = 1;
	int _autoDirection = +1;
	bool _isAnimating = false;
	int _lastAnimDirection = 0;
	int _jumpTarget = 0;

	QLabel* _slotFarLeft = nullptr;
	QLabel* _slotLeft = nullptr;
	QLabel* _slotCenter = nullptr;
	QLabel* _slotRight = nullptr;
	QLabel* _slotFarRight = nullptr;

	std::vector<QPushButton*> _dots;

	QParallelAnimationGroup* _slideGroup = nullptr;       // 相邻翻页
	QTimer* _autoTimer = nullptr;
};
