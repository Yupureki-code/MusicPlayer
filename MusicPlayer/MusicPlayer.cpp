#include "stdafx.h"
#include "MusicPlayer.h"
#include "Carousel.h"
#include "Playlist.h"
#include "BackgroundBlur.h"

void MusicPlayer::InitCarousels()
{
    Carousel* c1 = ui.recommendSongContent1;
    std::vector<SongPage> v;
    v.push_back({ QPixmap(":/image/image/testImage1.jpg"),QString("test1") });
    v.push_back({ QPixmap(":/image/image/testImage1.jpg"),QString("test1") });
    v.push_back({ QPixmap(":/image/image/testImage1.jpg"),QString("test1") });
    v.push_back({ QPixmap(":/image/image/testImage1.jpg"),QString("test1") });
    v.push_back({ QPixmap(":/image/image/testImage1.jpg"),QString("test1") });
    v.push_back({ QPixmap(":/image/image/testImage1.jpg"),QString("test1") });

    c1->setPages(v);

    // 初始化广告轮播卡片（DynamicCard）
    if (ui.recommendAdvertise) {
        std::vector<AdCard> ads;
        for (int i = 0; i < 6; ++i) {
            ads.push_back({ QPixmap(":/image/image/testImage1.jpg"), QString("ad%1").arg(i) });
        }
        ui.recommendAdvertise->setCards(ads);
    }
}

void MusicPlayer::InitPlayLists()
{
    std::vector<SongListInfo> v;
    v.push_back(SongListInfo(QString("1"), QString("1"), QString("1"), QString("1"), QTime(1, 1, 1)));
    v.push_back(SongListInfo(QString("2"), QString("2"), QString("2"), QString("2"), QTime(1, 1, 1)));
    v.push_back(SongListInfo(QString("3"), QString("3"), QString("3"), QString("3"), QTime(1, 1, 1)));
    v.push_back(SongListInfo(QString("4"), QString("4"), QString("4"), QString("4"), QTime(1, 1, 1)));
    v.push_back(SongListInfo(QString("5"), QString("5"), QString("5"), QString("5"), QTime(1, 1, 1)));
    v[0].is_VIP = true;
    v[1].is_SQ = true;
    v[2].is_HiRes = true;
    v[3].is_VIP = true; v[3].is_HiRes = true;
    v[4].is_VIP = true; v[4].is_HiRes = true; v[4].is_SQ = true; v[4].has_MV = true;
    ui.likeMusic->setSongList(v);
}

void MusicPlayer::InitPlayList()
{
    ui.likeMusic->setPlaylist(PlaylistInfo(QPixmap(":/image/image/testImage1.jpg"), QString("我喜欢的音乐"), QString("user1"), QDate(2026, 5, 23)));
}

void MusicPlayer::InitAudio()
{
    AudioEngine::GetInstance()->setParent(this);
    connect(AudioEngine::GetInstance(), &AudioEngine::songChanged,
        this, &MusicPlayer::on_song_changed);
    connect(AudioEngine::GetInstance(), &AudioEngine::positionChanged,
        this, &MusicPlayer::on_song_playing);
    std::vector<SongStruct> list;
    SongStruct song;
    song.url = QUrl::fromLocalFile("E:/C C++ Files/MusicPlayer/MusicPlayer/song/test.mp3");
    song.song_name = "知我";
    song.songer = "国风堂/哦漏";
    song.image = QPixmap("E:/C C++ Files/MusicPlayer/MusicPlayer/song/testImage.jpg");
    list.push_back(song);
    audio->setSongList(list);
    audio->setPlayMode(PlayMode::Sequential);
}

void MusicPlayer::InitDetailPage()
{
    _detailPage = new DetailPage(this);
    _detailPage->hide();
    _detailPage->setGeometry(0, 0, width(), height());

    _lyricsWidget = new LyricsWidget(_detailPage);
    _lyricsWidget->setGeometry(350, 80, width() - 400, height() - 180);

    connect(_detailPage, &DetailPage::backClicked, this, &MusicPlayer::closeDetailPage);
    connect(_detailPage, &DetailPage::playClicked, this, &MusicPlayer::on_play_clicked);

    connect(AudioEngine::GetInstance(), &AudioEngine::positionChanged,
        _lyricsWidget, &LyricsWidget::updatePosition);

    ui.songImage->installEventFilter(this);
}

bool MusicPlayer::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == ui.songImage && event->type() == QEvent::MouseButtonPress)
    {
        openDetailPage();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

MusicPlayer::MusicPlayer(QWidget *parent)
    : QWidget(parent),_is_seeking(false)
{
    InitConfig();
    ui.setupUi(this);
    InitUi();
    InitCarousels();
    InitPlayList();
    InitPlayLists();
    InitAudio();
    InitDetailPage();
}

void MusicPlayer::InitUi()
{
    // 取消Windows标题栏
    this->setWindowFlag(Qt::WindowType::FramelessWindowHint);
    // 设置窗口透明
    setAttribute(Qt::WA_TranslucentBackground, true);

    // 移除原来给阴影预留的 9px margin，由 paintEvent 统一绘制阴影和背景
    ui.verticalLayout_3->setContentsMargins(0, 0, 0, 0);
    // background 设为透明，由 MusicPlayer::paintEvent 统一画白色背景和阴影
    ui.background->setStyleSheet("background: transparent;");

    // 设置搜索栏图标
    QLineEdit* lineEdit = ui.searchLine;
    lineEdit->setPlaceholderText("搜索...");
    QAction* searchAction = new QAction(this);
    searchAction->setIcon(QIcon(":/image/image/find.png"));
    lineEdit->addAction(searchAction, QLineEdit::LeadingPosition);
    lineEdit->setStyleSheet("QLineEdit { padding-left: 20px; }");
    connect(searchAction, &QAction::triggered, this, []() {
        qDebug() << "搜索图标被点击";
        });
    // 音量滑块（QSlider 用 sliderPressed/sliderReleased，不能靠自动连接命名）
    connect(ui.songBar, &QSlider::sliderPressed, this, &MusicPlayer::on_songBar_pressed);
    connect(ui.songBar, &QSlider::sliderReleased, this, &MusicPlayer::on_songBar_released);
    // NavigateBT 图标已在其构造函数内自初始化
    // 设置初始选中按钮（与 QStackedWidget 第 0 页对应）
    if (ui.recommend) {
        SelectNavigateBT(ui.recommend);
    } else {
        ui.mainPlayBody->setCurrentIndex(_current_page_index);
    }
    // 画出分割线
    QPainter painter;
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen; pen.setWidth(10);
    pen.setColor(QColor("#726B6F"));
    painter.setPen(pen);
    int x0 = ui.navigateBox->x() + ui.navigateBox->width();
    int y0 = ui.navigateBox->y();
    int y1 = ui.navigateBox->y() + ui.navigateBox->height();
    painter.drawLine(QLine(x0, y0, x0, y1));
}

void MusicPlayer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();
    const int blur  = 12;   // 阴影扩散半径
    const int offX  = 4;    // 水平偏移
    const int offY  = 4;    // 垂直偏移

    // ---- 绘制阴影（从外到内，alpha 递增，模拟模糊） ----
    painter.setPen(Qt::NoPen);
    for (int i = blur; i >= 1; --i) {
        int alpha = 10 + (blur - i) * 12;   // 外层淡、内层浓
        alpha = qMin(alpha, 140);
        painter.setBrush(QColor(0, 0, 0, alpha));
        QRect shadow = r.adjusted(i, i, -i + offX, -i + offY);
        painter.drawRect(shadow);
    }

    // ---- 绘制白色内容背景（占据左上角，留出右下阴影） ----
    QRect content = r.adjusted(0, 0, -offX, -offY);
    painter.setBrush(Qt::white);
    painter.drawRect(content);

    QWidget::paintEvent(event);
}

void MusicPlayer::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (_detailPage)
    {
        _detailPage->setGeometry(0, 0, width(), height());
    }
    if (_lyricsWidget)
    {
        _lyricsWidget->setGeometry(350, 80, width() - 400, height() - 180);
    }
}

void MusicPlayer::on_minimize_clicked()
{
    showMinimized();
}

void MusicPlayer::on_maximize_clicked()
{
    if (this->isMaximized()) {
        this->showNormal();
    }
    else {
        this->showMaximized();
    }
}

void MusicPlayer::on_play_clicked()
{
    if (audio->isPlaying())
    {
        ui.play->setStyleSheet(QString("image: url(:/image/image/play.png);border:none;"));
        audio->pause();
    }
    else
    {
        ui.play->setStyleSheet(QString("image: url(:/image/image/stop.png);border:none;"));
        audio->play();
    }
}

void MusicPlayer::on_song_changed(const SongStruct& song)
{
    ui.songName->setText(song.song_name);
    ui.songerName->setText(song.songer);
    QTime t = QTime::fromMSecsSinceStartOfDay(static_cast<int>(song.time));
    ui.songTotalTime->setText(QString::number(t.minute()) + ":" + QString::number(t.second()));
    ui.songBar->setRange(0, song.time);

    if (!song.image.isNull())
    {
        QPixmap scaledImage = song.image.scaled(ui.songImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QLabel* imageLabel = ui.songImage->findChild<QLabel*>("songImageLabel");
        if (!imageLabel)
        {
            imageLabel = new QLabel(ui.songImage);
            imageLabel->setObjectName("songImageLabel");
            imageLabel->setGeometry(0, 0, ui.songImage->width(), ui.songImage->height());
        }
        imageLabel->setPixmap(scaledImage);

        if (_detailPage)
        {
            _detailPage->setCoverImage(song.image);
            _detailPage->setSongInfo(song.song_name, song.songer, "");
        }
    }

    if (_lyricsWidget)
    {
        QString lrcPath = "E:/C C++ Files/MusicPlayer/MusicPlayer/song/testLrc.lrc";
        _lyricsWidget->loadLyrics(lrcPath);
    }
}

void MusicPlayer::on_song_playing(qint64 positionMs)
{
    if (!_is_seeking)
        ui.songBar->setValue(static_cast<int>(positionMs));
    QTime t = QTime::fromMSecsSinceStartOfDay(static_cast<int>(positionMs));
    QString m = QString::number(t.minute());
    QString s = QString::number(t.second());
    if (t.minute() < 10)
        m = QString("0") + m;
    if (t.second() < 10)
        s = QString("0") + s;
    ui.songCurrentTime->setText(m + QString(":") + s);
}

void MusicPlayer::on_songBar_released()
{
    int positionMs = ui.songBar->value();
    QTime t = QTime::fromMSecsSinceStartOfDay(static_cast<int>(positionMs));
    QString m = QString::number(t.minute());
    QString s = QString::number(t.second());
    if (t.minute() < 10)
        m = QString("0") + m;
    if (t.second() < 10)
        s = QString("0") + s;
    ui.songCurrentTime->setText(m + QString(":") + s);
    AudioEngine::GetInstance()->seek(positionMs);
    _is_seeking = false;
}

void MusicPlayer::on_songBar_pressed()
{
    _is_seeking = true;
}

void MusicPlayer::on_songPlay_clicked()
{
    PlayMode mode = audio->getPlayMode();
    if (mode == PlayMode::Sequential)
    {
        ui.songPlay->setStyleSheet("image: url(:/image/image/loop.png);border:none; ");
        audio->setPlayMode(PlayMode::Loop);
    }
    else if (mode == PlayMode::Loop)
    {
        ui.songPlay->setStyleSheet("image: url(:/image/image/singleLoop.png);border:none; ");
        audio->setPlayMode(PlayMode::SingleLoop);
    }
    else if (mode == PlayMode::SingleLoop)
    {
        ui.songPlay->setStyleSheet("image: url(:/image/image/random.png);border:none; ");
        audio->setPlayMode(PlayMode::Random);
    }
    else
    {
        ui.songPlay->setStyleSheet("image: url(:/image/image/seq.png);border:none; ");
        audio->setPlayMode(PlayMode::Sequential);
    }
}

void MusicPlayer::InitConfig()
{
    // 初始化图片路径
    _images["like"] = ":/image/image/like.png";
    _images["recent"] = ":/image/image/recent.png";
    _images["local"] = ":/image/image/local.png";
    _images["myPodcast"] = ":/image/image/podcast.png";
    _images["podcast"] = ":/image/image/podcast.png";
    _images["favorite"] = ":/image/image/favorite.png";
    _images["recommendSongContent1Page1Image"] = ":/image/image/testImage1.jpg";

    //初始化页面下标
    _pages["recommend"] = 0;
    _pages["podcast"] = 1;
    _pages["musicHall"] = 2;
    _pages["like"] = 3;
    _pages["local"] = 4;
    _pages["recent"] = 5;
    _pages["myPodcast"] = 6;
    _pages["favorite"] = 7;
}

QString MusicPlayer::FindNavigateBTImages(NavigateBT* bt)
{
    return _images[bt->objectName()];
}

QString MusicPlayer::FindImages(QString name)
{
    return _images[name];
}

void MusicPlayer::SetCurrentPageIndex(const QString& page)
{
    _current_page_index = _pages[page];
    ui.mainPlayBody->setCurrentIndex(_current_page_index);
}

void MusicPlayer::SelectNavigateBT(NavigateBT* bt)
{
    if (!bt) return;
    // 取消旧按钮选中
    if (_currentSelectedBt && _currentSelectedBt != bt)
        _currentSelectedBt->setSelected(false);
    // 设置新按钮选中
    _currentSelectedBt = bt;
    bt->setSelected(true);
    // 同步页面
    SetCurrentPageIndex(bt->objectName());
}

void MusicPlayer::on_quit_clicked()
{
    close();
}

void MusicPlayer::openDetailPage()
{
    if (_is_detail_open || _is_animating)
        return;

    _is_animating = true;
    _is_detail_open = true;

    QPixmap blurredBg = BackgroundBlur::captureAndBlur(this, 20);
    _detailPage->setBackground(blurredBg);

    _detailPage->setGeometry(0, 0, width(), height());
    _detailPage->show();
    _detailPage->raise();

    QPropertyAnimation* menuAnim = new QPropertyAnimation(ui.menuBox, "pos");
    menuAnim->setDuration(500);
    menuAnim->setStartValue(ui.menuBox->pos());
    menuAnim->setEndValue(QPoint(ui.menuBox->x(), -ui.menuBox->height()));
    menuAnim->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation* bodyAnim = new QPropertyAnimation(ui.body, "pos");
    bodyAnim->setDuration(500);
    bodyAnim->setStartValue(ui.body->pos());
    bodyAnim->setEndValue(QPoint(ui.body->x(), -ui.body->height()));
    bodyAnim->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation* detailAnim = new QPropertyAnimation(_detailPage, "pos");
    detailAnim->setDuration(500);
    detailAnim->setStartValue(QPoint(0, height()));
    detailAnim->setEndValue(QPoint(0, 0));
    detailAnim->setEasingCurve(QEasingCurve::OutCubic);

    _openAnimGroup = new QParallelAnimationGroup(this);
    _openAnimGroup->addAnimation(menuAnim);
    _openAnimGroup->addAnimation(bodyAnim);
    _openAnimGroup->addAnimation(detailAnim);

    connect(_openAnimGroup, &QParallelAnimationGroup::finished, this, [this]() {
        _is_animating = false;
        ui.menuBox->hide();
        ui.body->hide();
    });

    _openAnimGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void MusicPlayer::closeDetailPage()
{
    if (!_is_detail_open || _is_animating)
        return;

    _is_animating = true;

    ui.menuBox->show();
    ui.body->show();

    QPropertyAnimation* menuAnim = new QPropertyAnimation(ui.menuBox, "pos");
    menuAnim->setDuration(500);
    menuAnim->setStartValue(ui.menuBox->pos());
    menuAnim->setEndValue(QPoint(ui.menuBox->x(), 0));
    menuAnim->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation* bodyAnim = new QPropertyAnimation(ui.body, "pos");
    bodyAnim->setDuration(500);
    bodyAnim->setStartValue(ui.body->pos());
    bodyAnim->setEndValue(QPoint(ui.body->x(), ui.menuBox->height()));
    bodyAnim->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation* detailAnim = new QPropertyAnimation(_detailPage, "pos");
    detailAnim->setDuration(500);
    detailAnim->setStartValue(_detailPage->pos());
    detailAnim->setEndValue(QPoint(0, height()));
    detailAnim->setEasingCurve(QEasingCurve::OutCubic);

    QParallelAnimationGroup* closeAnimGroup = new QParallelAnimationGroup(this);
    closeAnimGroup->addAnimation(menuAnim);
    closeAnimGroup->addAnimation(bodyAnim);
    closeAnimGroup->addAnimation(detailAnim);

    connect(closeAnimGroup, &QParallelAnimationGroup::finished, this, [this]() {
        _is_animating = false;
        _is_detail_open = false;
        _detailPage->hide();
    });

    closeAnimGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

MusicPlayer::~MusicPlayer()
{}

