#pragma once

#include <QWidget>
#include "ui_Carousel.h"
#include <vector>
#include <JumpingImage.h>

QT_BEGIN_NAMESPACE
class QPushButton;
class QPropertyAnimation;
QT_END_NAMESPACE

struct SongPage
{
	QPixmap _image;
	QString _name;
	SongPage() = default;
	SongPage(const QPixmap& image, const QString& name)
		: _image(image), _name(name) {}
};

class Carousel : public QWidget
{
	Q_OBJECT
public:
	Carousel(QWidget *parent = nullptr);
	~Carousel();
	void setPages(const std::vector<SongPage>& pages);
	void setImageWidth(int width);
	void setImageHeight(int height);
	void setPageWidth(int width);

protected:
	void enterEvent(QEnterEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private slots:
	void onLeftClicked();
	void onRightClicked();
	void checkMouseLeave();

private:
	void initPushButton();
	void initViewport();
	void refreshContent();
	void recalcVisibleCount();
	void repositionViewport();      // _viewport 居中 + _contentContainer 滑动
	void updateButtonState();
	void animateContentTo(int targetX);

	static constexpr int PAGE_SPACING = 20;
	static constexpr int BTN_SIZE = 40;

	Ui::CarouselClass ui;
	std::vector<SongPage> _pages;
	int _left = 0;
	int _visibleCount = 0;
	int _page_image_width = 130;
	int _page_width = 130;
	int _page_image_height = 130;

	QPushButton* _leftBtn = nullptr;
	QPushButton* _rightBtn = nullptr;
	QWidget* _viewport = nullptr;       // 裁剪视口，居中于 Carousel
	QWidget* _contentContainer = nullptr; // 滑动容器，父级为 _viewport
	QPropertyAnimation* _anim = nullptr;
};
