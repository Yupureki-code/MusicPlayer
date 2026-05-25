#include "stdafx.h"
#include "DynamicCard.h"
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>

void DynamicCard::initSlots()
{
    _slotFarLeft  = new QLabel(this);
    _slotLeft     = new QLabel(this);
    _slotCenter   = new QLabel(this);
    _slotRight    = new QLabel(this);
    _slotFarRight = new QLabel(this);

    for (auto* s : {_slotFarLeft, _slotLeft, _slotCenter, _slotRight, _slotFarRight}) {
        s->setScaledContents(true);
        s->setAlignment(Qt::AlignCenter);
    }
}

DynamicCard::DynamicCard(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    QLayout* old = layout();
    if (old) delete old;

    initSlots();

    _slideGroup = new QParallelAnimationGroup(this);
    connect(_slideGroup, &QParallelAnimationGroup::finished,
            this, &DynamicCard::onSlideFinished);

    _autoTimer = new QTimer(this);
    _autoTimer->setInterval(AUTOPLAY_INTERVAL);
    connect(_autoTimer, &QTimer::timeout,
            this, &DynamicCard::onAutoPlayTimeout);
}

void DynamicCard::setCards(const std::vector<AdCard>& cards)
{
    if (cards.size() < 3) return;

    _cards = cards;
    _currentIndex = 1;
    _autoDirection = +1;
    _isAnimating = false;
    _lastAnimDirection = 0;

    for (auto* d : _dots) { d->deleteLater(); }
    _dots.clear();
    int dotCount = qMax(0, (int)_cards.size() - 2);
    for (int i = 0; i < dotCount; ++i) {
        QPushButton* dot = new QPushButton(this);
        dot->setFixedSize(DOT_SIZE, DOT_SIZE);
        dot->setCursor(Qt::PointingHandCursor);
        dot->setFlat(true);
        dot->setFocusPolicy(Qt::NoFocus);
        connect(dot, &QPushButton::clicked, this, &DynamicCard::onDotClicked);
        dot->show();
        _dots.push_back(dot);
    }

    recalcGeometry();
    refreshSlotPixmaps();
    applyDotStyles();
    _autoTimer->start();
}

void DynamicCard::recalcGeometry()
{
    int W = width(), H = height();
    if (W <= 0 || H <= 0) return;

    // 共享尺寸
    _cW = (W - 40) * 5 / 7;
    _cH = H * 9 / 10;
    _sW = _cW * 5 / 6;
    _sH = _cH * 5 / 6;
    int vis = _cW / 5;
    _cX = (W - _cW) / 2;
    _cY = (H - _cH) / 2;
    _sY = (H - _sH) / 2;
    _sX_l = _cX - vis;
    _sX_r = _cX + _cW + vis - _sW;

    if (!_isAnimating) {
        _slotFarLeft ->setGeometry(QRect(-_sW, _sY, _sW, _sH));
        _slotLeft    ->setGeometry(QRect(_sX_l, _sY, _sW, _sH));
        _slotCenter  ->setGeometry(QRect(_cX, _cY, _cW, _cH));
        _slotRight   ->setGeometry(QRect(_sX_r, _sY, _sW, _sH));
        _slotFarRight->setGeometry(QRect(W, _sY, _sW, _sH));

        _slotLeft ->raise();
        _slotRight->raise();
        _slotCenter->raise();
    }
    layoutDots();
}

QRect DynamicCard::geometryForSlot(int slotIndex)
{
    switch (slotIndex) {
    case -2: return QRect(-_sW, _sY, _sW, _sH);
    case -1: return QRect(_sX_l, _sY, _sW, _sH);
    case  0: return QRect(_cX, _cY, _cW, _cH);
    case +1: return QRect(_sX_r, _sY, _sW, _sH);
    case +2: return QRect(width(), _sY, _sW, _sH);
    }
    return QRect();
}

void DynamicCard::layoutDots()
{
    int n = (int)_dots.size();
    if (n == 0) return;
    int spacing = 14;
    int totalW = n * DOT_SIZE + (n - 1) * spacing;
    int startX = (width() - totalW) / 2;
    int y = height() - DOT_SIZE - 12;
    for (int i = 0; i < n; ++i) {
        _dots[i]->move(startX + i * (DOT_SIZE + spacing), y);
        _dots[i]->raise();
    }
}

void DynamicCard::refreshSlotPixmaps()
{
    auto pix = [&](int i) -> QPixmap {
        return (i >= 0 && i < (int)_cards.size()) ? _cards[i].image : QPixmap();
    };
    _slotFarLeft ->setPixmap(pix(_currentIndex - 2));
    _slotLeft    ->setPixmap(pix(_currentIndex - 1));
    _slotCenter  ->setPixmap(pix(_currentIndex));
    _slotRight   ->setPixmap(pix(_currentIndex + 1));
    _slotFarRight->setPixmap(pix(_currentIndex + 2));
}

void DynamicCard::applyDotStyles()
{
    for (int i = 0; i < (int)_dots.size(); ++i) {
        bool active = (i == _currentIndex - 1);
        QString c = active ? "#E60026" : "#D0D0D0";
        _dots[i]->setStyleSheet(
            QString("QPushButton{background:%1; border:none; border-radius:%2px;}")
            .arg(c).arg(DOT_SIZE / 2));
    }
}

// ================================================================
//  相邻翻页动画：卡片汇聚到 / 从中部展开
// ================================================================
void DynamicCard::startAdjacentSlide(int direction)
{
    if (_isAnimating || _cards.size() < 3) return;
    if (direction == +1 && _currentIndex >= (int)_cards.size() - 2) return;
    if (direction == -1 && _currentIndex <= 1) return;

    _isAnimating = true;
    _lastAnimDirection = direction;
    _autoTimer->stop();

    // 收敛/展开位置
    QRect convergeL(_cX + _cW / 2 - _sW / 2, _sY, _sW, _sH);  // 左侧卡运动终点(被遮)
    QRect emergeR (_cX + _cW - _sW * 2 / 5, _sY, _sW, _sH);  // 右侧新卡起点

    // 预加载新入场的 far 卡
    auto safePix = [&](int idx) -> QPixmap {
        return (idx >= 0 && idx < (int)_cards.size()) ? _cards[idx].image : QPixmap();
    };

    if (direction == +1) {
        _slotFarRight->setPixmap(safePix(_currentIndex + 2));
        _slotFarRight->setGeometry(emergeR);
        _slotFarRight->show();

        // z-order
        _slotFarLeft->raise();
        _slotFarRight->raise();
        _slotLeft->raise();
        _slotRight->raise();
        _slotCenter->raise();
        for (auto* d : _dots) d->raise();

        while (_slideGroup->animationCount() > 0)
            _slideGroup->takeAnimation(0);

        auto addAnim = [&](QWidget* w, QRect from, QRect to) {
            QPropertyAnimation* a = new QPropertyAnimation(w, "geometry", _slideGroup);
            a->setStartValue(from);
            a->setEndValue(to);
            a->setDuration(SLIDE_DURATION);
            a->setEasingCurve(QEasingCurve::OutQuad);
            _slideGroup->addAnimation(a);
        };

        addAnim(_slotLeft,     geometryForSlot(-1), convergeL);                    // 左→中(被遮)
        addAnim(_slotCenter,   geometryForSlot( 0), geometryForSlot(-1));          // 中→左(缩小)
        addAnim(_slotRight,    geometryForSlot(+1), geometryForSlot( 0));          // 右→中(放大)
        addAnim(_slotFarRight, emergeR,              geometryForSlot(+1));          // 新卡从右中→右
    } else {
        QRect convergeR(_cX + _cW / 2 - _sW / 2, _sY, _sW, _sH);
        QRect emergeL (_cX + _sW * 2 / 5 - _sW, _sY, _sW, _sH);   // 左侧新卡起点

        _slotFarLeft->setPixmap(safePix(_currentIndex - 2));
        _slotFarLeft->setGeometry(emergeL);
        _slotFarLeft->show();

        _slotFarRight->raise();
        _slotFarLeft->raise();
        _slotRight->raise();
        _slotLeft->raise();
        _slotCenter->raise();
        for (auto* d : _dots) d->raise();

        while (_slideGroup->animationCount() > 0)
            _slideGroup->takeAnimation(0);

        auto addAnim = [&](QWidget* w, QRect from, QRect to) {
            QPropertyAnimation* a = new QPropertyAnimation(w, "geometry", _slideGroup);
            a->setStartValue(from);
            a->setEndValue(to);
            a->setDuration(SLIDE_DURATION);
            a->setEasingCurve(QEasingCurve::OutQuad);
            _slideGroup->addAnimation(a);
        };

        addAnim(_slotRight,   geometryForSlot(+1), convergeR);                     // 右→中(被遮)
        addAnim(_slotCenter,  geometryForSlot( 0), geometryForSlot(+1));           // 中→右(缩小)
        addAnim(_slotLeft,    geometryForSlot(-1), geometryForSlot( 0));           // 左→中(放大)
        addAnim(_slotFarLeft,  emergeL,             geometryForSlot(-1));           // 新卡从左中→左
    }

    _slideGroup->start();
}

void DynamicCard::onSlideFinished()
{
    _isAnimating = false;
    _currentIndex += _lastAnimDirection;

    QLabel* oldFarLeft  = _slotFarLeft;
    QLabel* oldLeft     = _slotLeft;
    QLabel* oldCenter   = _slotCenter;
    QLabel* oldRight    = _slotRight;
    QLabel* oldFarRight = _slotFarRight;

    if (_lastAnimDirection == +1) {
        _slotFarLeft  = oldLeft;
        _slotLeft     = oldCenter;
        _slotCenter   = oldRight;
        _slotRight    = oldFarRight;
        _slotFarRight = oldFarLeft;
    } else {
        _slotFarRight = oldRight;
        _slotRight    = oldCenter;
        _slotCenter   = oldLeft;
        _slotLeft     = oldFarLeft;
        _slotFarLeft  = oldFarRight;
    }

    auto safePix = [&](int idx) -> QPixmap {
        return (idx >= 0 && idx < (int)_cards.size()) ? _cards[idx].image : QPixmap();
    };
    _slotFarLeft ->setPixmap(safePix(_currentIndex - 2));
    _slotFarLeft ->setGeometry(geometryForSlot(-2));
    _slotFarLeft ->hide();
    _slotFarRight->setPixmap(safePix(_currentIndex + 2));
    _slotFarRight->setGeometry(geometryForSlot(+2));
    _slotFarRight->hide();

    // 中间三张只做 pixmap 刷新，加边界保护防止意外越界
    if (_currentIndex < 1 || _currentIndex >= (int)_cards.size() - 1) {
        qDebug() << "onSlideFinished: _currentIndex" << _currentIndex << "out of range, cards.size=" << _cards.size();
        _currentIndex = 1;
    }
    _slotLeft  ->setPixmap(_cards[_currentIndex - 1].image);
    _slotCenter->setPixmap(_cards[_currentIndex].image);
    _slotRight ->setPixmap(_cards[_currentIndex + 1].image);

    _slotLeft  ->raise();
    _slotRight ->raise();
    _slotCenter->raise();
    for (auto* d : _dots) d->raise();

    applyDotStyles();
    _autoTimer->start();
}

// ================================================================
//  跨步跳转动画：收缩 → 交换 → 展开（分两段串联）
// ================================================================
void DynamicCard::startJumpSlide(int targetIdx)
{
    if (_isAnimating || _cards.size() < 3) return;
    if (targetIdx < 1 || targetIdx >= (int)_cards.size() - 1) return;
    if (targetIdx == _currentIndex) return;

    _isAnimating = true;
    _jumpTarget = targetIdx;
    _autoTimer->stop();

    int W = width(), H = height();
    _cW = (W - 40) * 5 / 7;  _cH = H * 9 / 10;
    _sW = _cW * 5 / 6;       _sH = _cH * 5 / 6;
    int vis = _cW / 5;
    _cX = (W - _cW) / 2;     _cY = (H - _cH) / 2;
    _sY = (H - _sH) / 2;
    _sX_l = _cX - vis;       _sX_r = _cX + _cW + vis - _sW;

    // 汇聚目标
    QRect conv(_cX + _cW / 2 - _sW / 2, _sY, _sW, _sH);
    QRect shrinkCenter(_cX + _cW/40, _cY + _cH/40, _cW*19/20, _cH*19/20);

    // ---- Phase 1: 汇聚（200ms） ----
    QParallelAnimationGroup* phase1 = new QParallelAnimationGroup(this);

    _slotLeft  ->show();
    _slotCenter->show();
    _slotRight ->show();
    _slotLeft  ->raise();
    _slotRight ->raise();
    _slotCenter->raise();
    for (auto* d : _dots) d->raise();

    auto addP1 = [&](QWidget* w, QRect from, QRect to) {
        QPropertyAnimation* a = new QPropertyAnimation(w, "geometry", phase1);
        a->setStartValue(from);
        a->setEndValue(to);
        a->setDuration(JUMP_PHASE1_DURATION);
        a->setEasingCurve(QEasingCurve::OutQuad);
        phase1->addAnimation(a);
    };

    addP1(_slotLeft,   QRect(_sX_l, _sY, _sW, _sH), conv);
    addP1(_slotRight,  QRect(_sX_r, _sY, _sW, _sH), conv);
    addP1(_slotCenter, QRect(_cX, _cY, _cW, _cH),    shrinkCenter);

    connect(phase1, &QParallelAnimationGroup::finished,
            this, &DynamicCard::onJumpPhase1Finished,
            Qt::SingleShotConnection);  // 仅触发一次
    phase1->start();
}

void DynamicCard::onJumpPhase1Finished()
{
    _currentIndex = _jumpTarget;

    auto safePix = [&](int idx) -> QPixmap {
        return (idx >= 0 && idx < (int)_cards.size()) ? _cards[idx].image : QPixmap();
    };

    int W = width(), H = height();
    _cW = (W - 40) * 5 / 7;  _cH = H * 9 / 10;
    _sW = _cW * 5 / 6;       _sH = _cH * 5 / 6;
    int vis = _cW / 5;
    _cX = (W - _cW) / 2;     _cY = (H - _cH) / 2;
    _sY = (H - _sH) / 2;
    _sX_l = _cX - vis;       _sX_r = _cX + _cW + vis - _sW;

    // 展开起点（从中部出发）
    QRect emergeL (_cX + _sW * 2 / 5 - _sW, _sY, _sW, _sH);
    QRect emergeR (_cX + _cW - _sW * 2 / 5, _sY, _sW, _sH);

    // 先把远端 cards 加载到 far 槽位
    _slotFarLeft ->setPixmap(safePix(_currentIndex - 2));
    _slotFarLeft ->setGeometry(emergeL);
    _slotFarLeft ->show();
    _slotFarRight->setPixmap(safePix(_currentIndex + 2));
    _slotFarRight->setGeometry(emergeR);
    _slotFarRight->show();

    // 中间三张卡换 pixmap（当前在汇聚位置）
    _slotLeft  ->setPixmap(safePix(_currentIndex - 1));
    _slotCenter->setPixmap(safePix(_currentIndex));
    _slotRight ->setPixmap(safePix(_currentIndex + 1));

    _slotFarLeft ->raise();
    _slotLeft    ->raise();
    _slotRight   ->raise();
    _slotCenter  ->raise();
    _slotFarRight->raise();
    for (auto* d : _dots) d->raise();

    // ---- Phase 2: 展开（400ms） ----
    QParallelAnimationGroup* phase2 = new QParallelAnimationGroup(this);

    QRect locLeft (_sX_l, _sY, _sW, _sH);
    QRect locCenter(_cX, _cY, _cW, _cH);
    QRect locRight(_sX_r, _sY, _sW, _sH);
    QRect shrinkCenter(_cX + _cW/40, _cY + _cH/40, _cW*19/20, _cH*19/20);

    auto addP2 = [&](QWidget* w, QRect from, QRect to) {
        QPropertyAnimation* a = new QPropertyAnimation(w, "geometry", phase2);
        a->setStartValue(from);
        a->setEndValue(to);
        a->setDuration(JUMP_PHASE2_DURATION);
        a->setEasingCurve(QEasingCurve::OutQuad);
        phase2->addAnimation(a);
    };

    addP2(_slotFarLeft,  emergeL,      locLeft);
    addP2(_slotLeft,     locLeft,      QRect(-_sW, _sY, _sW, _sH));
    addP2(_slotCenter,   shrinkCenter, locCenter);
    addP2(_slotRight,    locRight,     QRect(W, _sY, _sW, _sH));
    addP2(_slotFarRight, emergeR,      locRight);

    connect(phase2, &QParallelAnimationGroup::finished, this, [this]() {
        _isAnimating = false;
        recalcGeometry();
        refreshSlotPixmaps();
        applyDotStyles();

        _slotFarLeft ->hide();
        _slotFarRight->hide();
        _slotLeft  ->raise();
        _slotRight ->raise();
        _slotCenter->raise();
        for (auto* d : _dots) d->raise();

        _autoTimer->start();
    }, Qt::SingleShotConnection);

    phase2->start();
}

// ================================================================
//  自动播 + dot 点击
// ================================================================
void DynamicCard::onAutoPlayTimeout()
{
    if (_isAnimating) return;
    if (_currentIndex >= (int)_cards.size() - 2 && _autoDirection == +1)
        _autoDirection = -1;
    else if (_currentIndex <= 1 && _autoDirection == -1)
        _autoDirection = +1;
    startAdjacentSlide(_autoDirection);
}

void DynamicCard::onDotClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    int idx = -1;
    for (int i = 0; i < (int)_dots.size(); ++i) {
        if (_dots[i] == btn) { idx = i; break; }
    }
    if (idx < 0) return;
    int target = idx + 1;
    if (target == _currentIndex) return;

    if (qAbs(target - _currentIndex) == 1)
        startAdjacentSlide(target > _currentIndex ? +1 : -1);
    else
        startJumpSlide(target);
}

void DynamicCard::resizeEvent(QResizeEvent* event)
{
    recalcGeometry();
    QWidget::resizeEvent(event);
}

void DynamicCard::enterEvent(QEnterEvent* event)
{
    _autoTimer->stop();
    QWidget::enterEvent(event);
}

void DynamicCard::leaveEvent(QEvent* event)
{
    _autoTimer->start();
    QWidget::leaveEvent(event);
}

DynamicCard::~DynamicCard()
{}
