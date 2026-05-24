#include "stdafx.h"
#include "Carousel.h"
#include <QPushButton>
#include <QPropertyAnimation>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QCursor>

void Carousel::initViewport()
{
    _viewport = new QWidget(this);
    _viewport->setAutoFillBackground(false);

    _contentContainer = new QWidget(_viewport);
    _contentContainer->move(0, 0);
    _contentContainer->setAutoFillBackground(false);
}

void Carousel::initPushButton()
{
    _leftBtn = new QPushButton(this);
    _rightBtn = new QPushButton(this);

    _leftBtn->setFixedSize(BTN_SIZE, BTN_SIZE);
    _rightBtn->setFixedSize(BTN_SIZE, BTN_SIZE);

    QString leftStyle = R"(
        QPushButton {
            background: rgba(0,0,0,100);
            border: none;
            border-radius: 20px;
            background-image: url(:/image/image/left.png);
            background-position: center;
            background-repeat: no-repeat;
        }
        QPushButton:hover {
            background: rgba(0,0,0,180);
        }
    )";
    QString rightStyle = R"(
        QPushButton {
            background: rgba(0,0,0,100);
            border: none;
            border-radius: 20px;
            background-image: url(:/image/image/right.png);
            background-position: center;
            background-repeat: no-repeat;
        }
        QPushButton:hover {
            background: rgba(0,0,0,180);
        }
    )";

    _leftBtn->setStyleSheet(leftStyle);
    _rightBtn->setStyleSheet(rightStyle);

    _leftBtn->hide();
    _rightBtn->hide();

    connect(_leftBtn, &QPushButton::clicked, this, &Carousel::onLeftClicked);
    connect(_rightBtn, &QPushButton::clicked, this, &Carousel::onRightClicked);
}

Carousel::Carousel(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    QLayout* old = layout();
    if (old) delete old;

    initViewport();
    initPushButton();

    _anim = new QPropertyAnimation(_contentContainer, "pos", this);
    _anim->setDuration(300);
    _anim->setEasingCurve(QEasingCurve::OutQuad);

    setMouseTracking(true);
}

void Carousel::setPages(const std::vector<SongPage>& pages)
{
    _pages = pages;
    _left = 0;
    recalcVisibleCount();
    refreshContent();
    updateButtonState();
}

void Carousel::setImageWidth(int width)
{
    _page_image_width = width;
}

void Carousel::setImageHeight(int height)
{
    _page_image_height = height;
}

void Carousel::setPageWidth(int width)
{
    _page_width = width;
}

void Carousel::recalcVisibleCount()
{
    int available = qMax(0, width());
    if (available < _page_width) {
        _visibleCount = 1;
    } else {
        _visibleCount = (available + PAGE_SPACING) / (_page_width + PAGE_SPACING);
        if (_visibleCount < 1) _visibleCount = 1;
    }

    if (!_pages.empty()) {
        int maxLeft = qMax(0, (int)_pages.size() - _visibleCount);
        if (_left > maxLeft) _left = maxLeft;
    }
}

void Carousel::refreshContent()
{
    // ---- 清理 _contentContainer 内部 ----
    QLayout* oldLayout = _contentContainer->layout();
    if (oldLayout) {
        while (QLayoutItem* item = oldLayout->takeAt(0)) {
            if (QWidget* w = item->widget()) {
                w->setParent(nullptr);
                delete w;
            }
            delete item;
        }
        delete oldLayout;
    }

    QHBoxLayout* hLayout = new QHBoxLayout(_contentContainer);
    hLayout->setSpacing(PAGE_SPACING);
    hLayout->setContentsMargins(0, 10, 0, 10);

    for (size_t i = 0; i < _pages.size(); ++i)
    {
        QWidget* page = new QWidget(_contentContainer);
        page->setFixedWidth(_page_width);
        QVBoxLayout* vLayout = new QVBoxLayout(page);
        vLayout->setSpacing(8);
        vLayout->setContentsMargins(0, 0, 0, 0);

        JumpingImage* image = new JumpingImage(page);
        image->setObjectName(QString("page%1Image").arg(i));
        image->setFixedSize(_page_image_width, _page_image_height);
        image->setImage(_pages[i]._image);
        vLayout->addWidget(image, 0, Qt::AlignHCenter);

        QLabel* label = new QLabel(page);
        label->setObjectName(QString("page%1Label").arg(i));
        label->setText(_pages[i]._name);
        label->setStyleSheet("font-size: 13px; color: #333333;");
        vLayout->addWidget(label, 0, Qt::AlignHCenter);

        hLayout->addWidget(page);
    }

    // _contentContainer 总宽 = 所有 page + spacing
    int n = (int)_pages.size();
    int totalContentWidth = n * _page_width + qMax(0, n - 1) * PAGE_SPACING;
    _contentContainer->setFixedSize(totalContentWidth, height());

    // _viewport 宽 = 可见页面总宽，通过 mask 精确裁剪
    int visibleW = _visibleCount * _page_width + qMax(0, _visibleCount - 1) * PAGE_SPACING;
    _viewport->setFixedSize(visibleW, height());
    _viewport->setMask(QRegion(0, 0, visibleW, height()));

    repositionViewport();
}

void Carousel::repositionViewport()
{
    if (_pages.empty()) return;

    int visibleW = _viewport->width();

    // _viewport 在 Carousel 内水平居中
    int viewportX = (width() - visibleW) / 2;
    _viewport->move(viewportX, 0);

    // _contentContainer 在 _viewport 内滑动
    int step = _page_width + PAGE_SPACING;
    int contentX = -_left * step;
    _contentContainer->move(contentX, 0);

    // 方向键定位在 _viewport 左右边缘（垂直居中）
    int btnY = height() / 2 - BTN_SIZE / 2;
    _leftBtn->move(viewportX, btnY);
    _rightBtn->move(viewportX + visibleW - BTN_SIZE, btnY);
}

void Carousel::updateButtonState()
{
    if (_pages.empty() || (int)_pages.size() <= _visibleCount) {
        _leftBtn->hide();
        _rightBtn->hide();
        return;
    }

    int maxLeft = qMax(0, (int)_pages.size() - _visibleCount);
    _leftBtn->setVisible(_left > 0);
    _rightBtn->setVisible(_left < maxLeft);
}

void Carousel::animateContentTo(int targetX)
{
    _anim->stop();
    _anim->setStartValue(_contentContainer->pos());
    _anim->setEndValue(QPoint(targetX, 0));
    _anim->start();
}

void Carousel::onLeftClicked()
{
    if (_left <= 0) return;
    _left--;
    int step = _page_width + PAGE_SPACING;
    animateContentTo(-_left * step);
    updateButtonState();
}

void Carousel::onRightClicked()
{
    int maxLeft = qMax(0, (int)_pages.size() - _visibleCount);
    if (_left >= maxLeft) return;
    _left++;
    int step = _page_width + PAGE_SPACING;
    animateContentTo(-_left * step);
    updateButtonState();
}

void Carousel::enterEvent(QEnterEvent* event)
{
    updateButtonState();
    QWidget::enterEvent(event);
}

void Carousel::leaveEvent(QEvent* event)
{
    QTimer::singleShot(80, this, &Carousel::checkMouseLeave);
    QWidget::leaveEvent(event);
}

void Carousel::checkMouseLeave()
{
    QPoint localPos = mapFromGlobal(QCursor::pos());
    if (!rect().contains(localPos)) {
        _leftBtn->hide();
        _rightBtn->hide();
    }
}

void Carousel::resizeEvent(QResizeEvent* event)
{
    if (!_viewport || !_contentContainer || _pages.empty()) {
        QWidget::resizeEvent(event);
        return;
    }

    recalcVisibleCount();

    // _viewport 高度跟随 Carousel，宽度重新计算
    int visibleW = _visibleCount * _page_width + qMax(0, _visibleCount - 1) * PAGE_SPACING;
    _viewport->setFixedSize(visibleW, height());
    _viewport->setMask(QRegion(0, 0, visibleW, height()));

    // _contentContainer 高度跟随
    _contentContainer->setFixedHeight(height());

    repositionViewport();
    updateButtonState();
    QWidget::resizeEvent(event);
}

Carousel::~Carousel()
{}
