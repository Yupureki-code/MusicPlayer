#include "stdafx.h"
#include "NavigateBT.h"
#include "MusicPlayer.h"

void NavigateBT::resolveParent()
{
    if (_parent) return;
    QWidget* p = parentWidget();
    while (p) {
        _parent = qobject_cast<MusicPlayer*>(p);
        if (_parent) break;
        p = p->parentWidget();
    }
}

void NavigateBT::InitIcon()
{
    resolveParent();
    if (!_parent)
        return;

    QString name = objectName() + "Icon";
    auto icon = findChild<QLabel*>(name);
    if (!icon)
        return;

    QString imagePath = _parent->FindNavigateBTImages(this);
    if (imagePath.isEmpty())
        return;

    icon->setPixmap(QPixmap(imagePath));
    icon->setScaledContents(true); // 控制图标大小
}

void NavigateBT::InitStyle()
{
    setMouseTracking(true);
    setAutoFillBackground(false);
    setMinimumSize(120, 40);
}

void NavigateBT::InitLabelTransparent()
{
    // 让所有子 QLabel 背景透明，文字颜色适配暗色背景
    for (QLabel* label : findChildren<QLabel*>()) {
        label->setStyleSheet("background: transparent; color: rgb(200, 200, 200); font-size: 16px;");
    }
}

// [毛玻璃效果 - 为后续皮肤切换保留，当前未使用]
void NavigateBT::captureBackground()
{
    if (m_bgCached)
        return;

    QWidget* p = parentWidget();
    if (!p || !p->isVisible() || p->size().isEmpty())
        return;

    // 使用grab()截取父控件背景
    QPixmap parentPix = p->grab();
    if (parentPix.isNull())
        return;

    QPoint posInParent = mapToParent(QPoint(0, 0));
    QRect fragmentRect(posInParent.x(), posInParent.y(), width(), height());
    m_bgNormal = parentPix.copy(fragmentRect);
    if (m_bgNormal.isNull())
        return;

    m_bgHover = applyBlur(m_bgNormal, 8);
    m_bgPressed = applyBlur(m_bgNormal, 15);
    m_bgCached = true;
}

void NavigateBT::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 圆角矩形路径
    QPainterPath path;
    int radius = 8;
    path.addRoundedRect(rect(), radius, radius);

    // 裁剪为圆角
    painter.setClipPath(path);

    // 绘制背景
    if (m_selected && !m_bgPressed.isNull()) {
        painter.drawPixmap(0, 0, m_bgPressed);
        painter.fillRect(rect(), QColor(255, 255, 255, 60));
    } else if (m_blurRadius > 0 && !m_bgHover.isNull()) {
        painter.drawPixmap(0, 0, m_bgHover);
        painter.fillRect(rect(), QColor(255, 255, 255, 30));
    }

    QWidget::paintEvent(event);
}

void NavigateBT::updateLabelStyle()
{
    QString name = objectName() + "Text";
    auto label = findChild<QLabel*>(name);
    if (!label)
        return;

    QFont f = label->font();
    QColor color;

    if (m_selected) {
        // 选中状态：白色加粗
        color = QColor(255, 255, 255);
        f.setBold(true);
    } else if (m_blurRadius > 0) {
        // 悬停状态：白色加粗
        color = QColor(255, 255, 255);
        f.setBold(true);
    } else {
        // 正常状态：浅灰色普通（适配暗色背景）
        color = QColor(200, 200, 200);
        f.setBold(false);
    }

    label->setFont(f);
    QPalette pal = label->palette();
    pal.setColor(QPalette::WindowText, color);
    label->setPalette(pal);
}

void NavigateBT::enterEvent(QEnterEvent* event) {
    if (m_blurRadius > 0) {
        // 已经在悬停/按下状态，只是鼠标在内部子控件间穿梭
        QWidget::enterEvent(event);
        return;
    }
    m_blurRadius = 8;
    m_pressed = false;
    updateLabelStyle();
    update();
    QWidget::enterEvent(event);
}

void NavigateBT::leaveEvent(QEvent* event) {
    m_blurRadius = 0;
    m_pressed = false;
    updateLabelStyle();
    update();
    QWidget::leaveEvent(event);
}

void NavigateBT::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_blurRadius = 15;
        m_pressed = true;
        updateLabelStyle();
        update();
    }
    QWidget::mousePressEvent(event);
}

void NavigateBT::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        bool inside = rect().contains(event->pos());
        // 鼠标释放后，如果仍在控件内则恢复悬停状态，否则恢复普通
        m_blurRadius = inside ? 8 : 0;
        m_pressed = false;
        updateLabelStyle();
        update();

        // 在按钮内释放才算一次有效点击，交由 MusicPlayer 做互斥选中 + 页面切换
        if (inside) {
            resolveParent();
            if (_parent)
                _parent->SelectNavigateBT(this);
        }
    }
    QWidget::mouseReleaseEvent(event);
}

void NavigateBT::setSelected(bool selected)
{
    m_selected = selected;
    updateLabelStyle();
    update();
}

NavigateBT::NavigateBT(QWidget* parent)
    : QWidget(parent)
    , _parent(nullptr)
{
    ui.setupUi(this);
    InitStyle();
    // 延迟初始化：等 MusicPlayer 完全构造、子控件树建立完毕后再执行
    QMetaObject::invokeMethod(this, &NavigateBT::InitIcon, Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, &NavigateBT::InitLabelTransparent, Qt::QueuedConnection);
}

void NavigateBT::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    // 首次显示时延迟捕获背景，避免在paintEvent中grab导致无限递归
    if (!m_bgCached) {
        QTimer::singleShot(0, this, [this]() { captureBackground(); });
    }
}

// [毛玻璃效果 - 为后续皮肤切换保留，当前未使用]
QPixmap NavigateBT::applyBlur(const QPixmap& src, int radius) {
    if (radius <= 0 || src.isNull()) return src;

    QGraphicsBlurEffect* effect = new QGraphicsBlurEffect;
    effect->setBlurRadius(radius);

    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(src);
    item->setGraphicsEffect(effect);

    QGraphicsScene scene;
    scene.addItem(item);

    QPixmap result(src.size());
    result.fill(Qt::transparent);
    QPainter p(&result);
    scene.render(&p, QRectF(), QRectF(0, 0, src.width(), src.height()));
    p.end();

    // scene 析构时自动 delete item，item 析构时自动 delete effect
    return result;
}

NavigateBT::~NavigateBT()
{}
