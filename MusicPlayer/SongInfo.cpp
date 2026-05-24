#include "stdafx.h"
#include "SongInfo.h"

SongInfo::SongInfo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void SongInfo::setInfo(const SongListInfo& info)
{
	ui.code->setText(info.code);
	ui.title->setText(info.title);
	ui.songer->setText(info.songer);
	ui.album->setText(info.album);
	ui.time->setText(QString::number(info.time.minute()) + QString(":") + QString::number(info.time.second()));
	QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(ui.label->layout());
	if (info.is_VIP)
	{
		QLabel* VIP = new QLabel("VIP");
		VIP->setMinimumSize(QSize(26, 15));
		VIP->setMaximumSize(QSize(26, 15));
		VIP->setStyleSheet(QString("font-size:10px;color:#F0AF75;border-radius:4px;border: 2px solid #F0AF75;"));
		layout->addWidget(VIP);
	}
	if (info.is_SQ)
	{
		QLabel* SQ = new QLabel("SQ");
		SQ->setMinimumSize(QSize(22, 15));
		SQ->setMaximumSize(QSize(22, 15));
		SQ->setStyleSheet(QString("font-size:10px;color:#B53B43;border-radius:4px;border: 2px solid #B53B43;"));
		layout->addWidget(SQ);
	}
	if (info.is_HiRes)
	{
		QLabel* HiRes = new QLabel("Hi-Res");
		HiRes->setMinimumSize(QSize(40, 15));
		HiRes->setMaximumSize(QSize(40, 15));
		HiRes->setStyleSheet(QString("font-size:10px;color:#B53B43;border-radius:4px;border: 2px solid #B53B43;"));
		layout->addWidget(HiRes);
	}
	if (info.has_MV)
	{
		QLabel* MV = new QLabel("MV");
		MV->setMinimumSize(QSize(22, 15));
		MV->setMaximumSize(QSize(22, 15));
		MV->setStyleSheet(QString("font-size:10px;color:#B53B43;border-radius:4px;border: 2px solid #B53B43;"));
		layout->addWidget(MV);
	}
	layout->addStretch();
}

SongInfo::~SongInfo()
{}

