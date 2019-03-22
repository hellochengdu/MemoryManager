#include "MemroyPlay.h"
#include <QComboBox>
#include <QLabel>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include <QTimer>
#include <QToolButton>
#include <QtMultimedia>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <windows.h>
#include <WindowTitle.h>
#include <QTimer>
#include <QTreeWidget>
#include <QMessageBox>
#include <QAction>
#include <QHeaderView>
#include "CustomSlider.h"
#include <time.h>
#include "dhnetsdk.h"
#include "dhconfigsdk.h"
#define  MPWIDTH 1000
#define  MPHEIGT 800
MemroyPlay::MemroyPlay(QWidget *parent ,char* ip, char* devicetype,long longid)
{
	logID = longid;
	strcpy(DeviceType, devicetype);
	this->setWindowTitle(QString::fromLocal8Bit(ip));
	this->setWindowFlags(Qt::FramelessWindowHint);
	/*WindowTitle* wTitle = new WindowTitle(this, MPWIDTH, 99);
	wTitle->setGeometry(0, 0, MPWIDTH, 99);
	wTitle->show();*/
	QToolButton* closeButton = new QToolButton(this);
	closeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* pActions = new QAction(closeButton);
	pActions->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/close_wnd_h.png"));
	closeButton->setIconSize(QSize(100, 50));
	closeButton->setDefaultAction(pActions);
	closeButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	closeButton->setGeometry(MPWIDTH - 50, 0, 50, 30);
	//closeButton->show();
	connect(closeButton, &QToolButton::clicked, this, &QWidget::close);
	connect(closeButton, &QToolButton::clicked, this, &MemroyPlay::parentshow);
	this->resize(MPWIDTH, MPHEIGT);
	ipc = new QComboBox(this);
	ipc->setGeometry(7, 54, 100, 20);
	QStringList* ipcstring=new QStringList;
	GetIpNumbers(ipcstring);
	ipc->addItems(*ipcstring);
	QLabel* sLabel = new QLabel(this);
	sLabel->setGeometry(5, 75, 50, 20);
	sLabel->setText(QString::fromLocal8Bit("开始时间"));
	QLabel* eLabel = new QLabel(this);
	eLabel->setGeometry(5, 95, 50, 20);
	eLabel->setText(QString::fromLocal8Bit("结束时间"));
    sTime = new QDateTimeEdit(QDateTime::currentDateTime().addDays(-1),this);
	sTime->setMaximumDateTime(QDateTime::currentDateTime());
	sTime->setGeometry(80, 75, 170, 20);
	eTime = new QDateTimeEdit(QDateTime::currentDateTime(),this);
	eTime->setMaximumDateTime(QDateTime::currentDateTime());
	eTime->setGeometry(80, 95, 170, 20);
	QCalendarWidget* apartof = new QCalendarWidget(this);
	apartof->setGeometry(7, 115, 243, 300);
	connect(apartof, SIGNAL(clicked(const QDate)), this, SLOT(setTime(const QDate)));
	doubleTimer = new QTimer(this);
	doubleTimer->setSingleShot(true);
	doubleTimer->setTimerType(Qt::PreciseTimer);
	VideoWidget = new QVideoWidget(this);
	//VideoWidget->setStyleSheet("background-color:rgb(255,0,0);");
	VideoWidget->setGeometry(252, 54, 746, 646);

	playbackSlider = new CustomRectItem;
	playbackSlider->setParent(this);
	playbackSlider->setGeometry(252, 705, 746, 30);
	playbackSlider->setEnabled(false);
	connect(playbackSlider, SIGNAL(sliderReleased()), this, SLOT(setPos()));
	connect(playbackSlider, SIGNAL(CustomRectItemMousePress()), this, SLOT(setPos()));
	
	pauseButton = new QToolButton(this);
	pauseButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* pauseAction = new QAction(pauseButton);
	pauseAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/stop_play.png"));
	pauseButton->setIconSize(QSize(20, 20));
	pauseButton->setDefaultAction(pauseAction);
	pauseButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	pauseButton->setGeometry(440, 765, 20, 20);
	pauseButton->setEnabled(false);

	fastButton = new QToolButton(this);
	fastButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* fastAction = new QAction(fastButton);
	fastAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/speed_up.png"));
	fastButton->setIconSize(QSize(50, 50));
	fastButton->setDefaultAction(fastAction);
	fastButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	fastButton->setGeometry(525, 750, 50, 50);
	fastButton->setEnabled(false);

	slowButton = new QToolButton(this);
	slowButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* slowAction = new QAction(slowButton);
	slowAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/speed_down.png"));
	slowButton->setIconSize((QSize(50, 50)));
	slowButton->setDefaultAction(slowAction);
	slowButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	slowButton->setGeometry(330, 750, 50, 50);
	slowButton->setEnabled(false);

	/*normalButton = new QToolButton(VideoWidget);
	normalButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* normalAction = new QAction(normalButton);
	normalAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/stop_play.png"));
	normalButton->setIconSize(QSize(50, 50));
	normalButton->setDefaultAction(normalAction);
	normalButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent");*/

	playbacktrueButton = new QToolButton(this);
	playbacktrueButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* playbacktrueAction = new QAction(playbacktrueButton);
	playbacktrueAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/play_back.png"));
	playbacktrueButton->setIconSize(QSize(20, 20));
	playbacktrueButton->setDefaultAction(playbacktrueAction);
	playbacktrueButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent");
	playbacktrueButton->setGeometry(252, 765, 20, 20);
	playbacktrueButton->setEnabled(false);

	playbackfalseButton = new QToolButton(this);
	playbackfalseButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* playbackfalseAction = new QAction(playbackfalseButton);
	playbackfalseAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/play_advance.png"));
	playbackfalseButton->setIconSize(QSize(20, 20));
	playbackfalseButton->setDefaultAction(playbackfalseAction);
	playbackfalseButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent");
	playbackfalseButton->setGeometry(635, 765, 20, 20);
	playbackfalseButton->setEnabled(false);
	
	otherButton = new QToolButton(this);
	otherButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* otherAction = new QAction(otherButton);
	otherAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/terminate.png"));
	otherButton->setIconSize(QSize(30, 30));
	otherButton->setDefaultAction(otherAction);
	otherButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent");
	otherButton->setGeometry(740, 760, 30, 30);
	otherButton->setEnabled(false);

	voideButton = new QToolButton(this);
	voideButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* voideAction = new QAction(voideButton);
	voideAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/voice.png"));
	voideButton->setIconSize(QSize(30, 30));
	voideButton->setDefaultAction(voideAction);
	voideButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent");
	voideButton->setGeometry(845, 760, 30, 30);
	voideButton->setEnabled(false);

	videoControl = new CustomRectItem;
	videoControl->setGeometry(875, 760, 125, 30);
	videoControl->setParent(this);
	videoControl->setEnabled(false);
	connect(videoControl, SIGNAL(sliderReleased()), this, SLOT(setvideoPos()));
	//connect(videoControl, SIGNAL(CustomRectItemMousePress()), this, SLOT(setvideoPos()));

	QToolButton* sureButton = new QToolButton(this);
	sureButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* sureAction = new QAction(sureButton);
	sureAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/play_start.png"));
	sureButton->setIconSize(QSize(50, 30));
	sureButton->setDefaultAction(sureAction);
	sureButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	sureButton->setGeometry(130, 620, 50, 30);

	QToolButton* seekButton = new QToolButton(this);
	seekButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* seekAction = new QAction(sureButton);
	seekAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/seek.png"));
	seekButton->setIconSize(QSize(50, 30));
	seekButton->setDefaultAction(seekAction);
	seekButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	seekButton->setGeometry(70, 620, 50, 30);

	connect(seekButton, SIGNAL(clicked()), this, SLOT(seekFunc()));
	connect(sureButton, SIGNAL(clicked()), this, SLOT(play()));
	connect(doubleTimer,SIGNAL(timeout()), this,SLOT(processOneThing()));
	connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseFunc()));
	connect(fastButton, SIGNAL(clicked()), this, SLOT(fastFunc()));
	connect(slowButton, SIGNAL(clicked()), this, SLOT(slowFunc()));
	connect(playbacktrueButton, SIGNAL(clicked()), this, SLOT(playbackFunc()));
	connect(playbackfalseButton, SIGNAL(clicked()), this, SLOT(playbackfFunc()));
	connect(voideButton, SIGNAL(clicked()), this, SLOT(voideFunc()));
	connect(otherButton, SIGNAL(clicked()), this, SLOT(otherFunc()));
	playpauseState = false;
	playDirection = false;
	playVideo = true;
	m_lhPlayBack = -1;
	TimeSetting = new QTimer(this);
	fileWidget = new QTreeWidget(this);
	fileWidget->setGeometry(7, 415, 245, 200);
	fileWidget->setColumnCount(2);
	fileWidget->setHeaderLabel(QString::fromLocal8Bit("	录像文件信息"));
	fileWidget->hideColumn(1);
	QHeaderView *pHeader = fileWidget->header();
	pHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
	pHeader->setStretchLastSection(false);
	connect(TimeSetting, SIGNAL(timeout()), this, SLOT(Timefunc()));
	connect(fileWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(DoubleClickedFunc(QTreeWidgetItem*, int)));
	this->show();
}
void MemroyPlay::pauseFunc()
{
	int error;
	if (!playpauseState&&m_lhPlayBack!=-1)
	{
		if (strncmp(DeviceType,"DH",2)==0)
		{
			if (CLIENT_PausePlayBack(m_lhPlayBack,1))
			{
				QAction* pauseAction = pauseButton->defaultAction();
				pauseAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/start_play.png"));
				pauseButton->setIconSize(QSize(20, 20));
				pauseButton->setDefaultAction(pauseAction);
				pauseButton->update();
				playpauseState = true;
				return;
			}
			else {
				QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("暂停回放失败"));
			}
		} 
		else
		{
			if (NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYPAUSE, NULL, 0, NULL, NULL))
			{
				QAction* pauseAction = pauseButton->defaultAction();
				pauseAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/start_play.png"));
				pauseButton->setIconSize(QSize(20, 20));
				pauseButton->setDefaultAction(pauseAction);
				pauseButton->update();
				playpauseState = true;
				return;
			}
			else {
				QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("暂停回放失败"));
			}
		}
	}
	if (playpauseState&&m_lhPlayBack!=-1)
	{
		if (strncmp(DeviceType,"DH",2)==0)
		{
			if (CLIENT_PausePlayBack(m_lhPlayBack, 0))
			{
				QAction* pauseAction = pauseButton->defaultAction();
				pauseAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/stop_play.png"));
				pauseButton->setIconSize(QSize(20, 20));
				pauseButton->setDefaultAction(pauseAction);
				pauseButton->update();
				playpauseState = false;
				return;
			}
			else {
				QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("暂停回放失败"));
			}
		} 
		else
		{
			if (NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYRESTART, NULL, 0, NULL, NULL))
			{
				QAction* pauseAction = pauseButton->defaultAction();
				pauseAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/stop_play.png"));
				pauseButton->setIconSize(QSize(20, 20));
				pauseButton->setDefaultAction(pauseAction);
				pauseButton->update();
				playpauseState = false;
				return;
			}
			else {
				QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("开始回放失败"));
			}
		}
	}
	error = NET_DVR_GetLastError();
}
void MemroyPlay::fastFunc()
{
	int error;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		if (!CLIENT_FastPlayBack(m_lhPlayBack))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("改变回放速度失败"));
		}
	} 
	else
	{
		if (!NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYFAST, NULL, 0, NULL, NULL))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("改变回放速度失败"));
			error = NET_DVR_GetLastError();
		}
	}
}
void MemroyPlay::slowFunc()
{
	int error;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		if (!CLIENT_SlowPlayBack(m_lhPlayBack))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("改变回放速度失败"));
		}
	} 
	else
	{
		if (!NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYSLOW, NULL, 0, NULL, NULL))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("改变回放速度失败"));
			error = NET_DVR_GetLastError();
		}
	}
}
//正放转倒放
void MemroyPlay::playbackFunc()
{
	int error;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		if (!CLIENT_PlayBackControlDirection(m_lhPlayBack,TRUE))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("改变回放方向失败"));
			return;
		}
	} 
	else
	{
		if (!NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAY_REVERSE, NULL, 0, NULL, NULL))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("改变回放方向失败"));
			error = NET_DVR_GetLastError();
			return;
		}
	}
	playbacktrueButton->setEnabled(false);
	playbackfalseButton->setEnabled(true);
	playpauseState = true;
}
//倒放转正放
void MemroyPlay::playbackfFunc()
{
	int error;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		if (!CLIENT_PlayBackControlDirection(m_lhPlayBack,FALSE))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("改变回放方向失败"));
			return;
		}
	} 
	else
	{
		if (!NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAY_FORWARD, NULL, 0, NULL, NULL))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("改变回放方向失败"));
			error = NET_DVR_GetLastError();
			return;
		}
	}
	playbackfalseButton->setEnabled(false);
	playbacktrueButton->setEnabled(true);
	playpauseState = true;
}
void MemroyPlay::voideFunc()
{
	int error;
	if (!playVideo)
	{
		if (strncmp(DeviceType, "DH", 2) == 0)
		{
			if (!CLIENT_OpenSound(m_lhPlayBack))
			{
				QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("打开声音失败"));
				error = CLIENT_GetLastError() & 0x7fffffff;
				return;
			}
		}
		else
		{
			if (!NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYSTARTAUDIO, NULL, 0, NULL, NULL))
			{
				QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("打开声音失败"));
				error = NET_DVR_GetLastError();
				return;
			}
		}
		QAction* videoAction = voideButton->defaultAction();
		videoAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/voice.png"));
		voideButton->setIconSize(QSize(20, 20));
		voideButton->setDefaultAction(videoAction);
		voideButton->update();
		videoControl->setEnabled(true);
		playVideo = true;
		return;
	}
	if (strncmp(DeviceType, "DH", 2) == 0)
	{
		if (!CLIENT_CloseSound())
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("关闭声音失败"));
			error = CLIENT_GetLastError() & 0x7fffffff;
			return;
		}
	}
	else
	{
		if (!NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYSTOPAUDIO, NULL, 0, NULL, NULL))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("关闭声音失败"));
			error = NET_DVR_GetLastError();
			return;
		}
	}
		QAction* videoAction = voideButton->defaultAction();
		videoAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/close_voice.png"));
		voideButton->setIconSize(QSize(20, 20));
		voideButton->setDefaultAction(videoAction);
		voideButton->update();
		videoControl->setEnabled(false);
		playVideo = false;
}
//停止播放
void MemroyPlay::otherFunc()
{
	int error;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		if (!CLIENT_StopPlayBack(m_lhPlayBack))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("停止回放失败"));
			error = CLIENT_GetLastError() & 0x7fffffff;
			return;
		}
	} 
	else
	{
		if (!NET_DVR_StopPlayBack(m_lhPlayBack))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("停止回放失败"));
			error = NET_DVR_GetLastError();
			return;
		}
	}
	pauseButton->setEnabled(false);
	fastButton->setEnabled(false);
	slowButton->setEnabled(false);
	playbacktrueButton->setEnabled(false);
	playbackfalseButton->setEnabled(false);
	voideButton->setEnabled(false);
	otherButton->setEnabled(false);
	playbackSlider->setEnabled(false);
	videoControl->setEnabled(false);
	playbackSlider->setValue(0);
	VideoWidget->hide();
	TimeSetting->stop();
	if (playVideo)
	{
		QAction* videoAction = voideButton->defaultAction();
		videoAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/close_voice.png"));
		voideButton->setIconSize(QSize(20, 20));
		voideButton->setDefaultAction(videoAction);
		voideButton->update();
		playVideo = false;
	}
	m_lhPlayBack = -1;
}
void MemroyPlay::setPos()
{
	int error= playbackSlider->value();
	QString strStartTime = QString("%1-%2-%3 %4:%5:%6").arg(sTime->date().year()).arg(sTime->date().month(), 2, 10, QChar('0')).arg(sTime->date().day(), 2, 10, QChar('0')).arg(sTime->time().hour(), 2, 10, QChar('0')).arg(sTime->time().minute(), 2, 10, QChar('0')).arg(sTime->time().second(), 2, 10, QChar('0'));
	QString strEndTime = QString("%1-%2-%3 %4:%5:%6").arg(eTime->date().year()).arg(eTime->date().month(), 2, 10, QChar('0')).arg(eTime->date().day(), 2, 10, QChar('0')).arg(eTime->time().hour(), 2, 10, QChar('0')).arg(eTime->time().minute(), 2, 10, QChar('0')).arg(eTime->time().second(), 2, 10, QChar('0'));
	QDateTime startTime = QDateTime::fromString(strStartTime, "yyyy-MM-dd hh:mm:ss");
	QDateTime endTime = QDateTime::fromString(strEndTime, "yyyy-MM-dd hh:mm:ss");

	//总秒数
	UINT64 totalSec = startTime.secsTo(endTime);
	UINT64 addSec = ((float)playbackSlider->value() / 1000) * totalSec;
	startTime = startTime.addSecs(addSec);//加上拖拽的秒数

	NET_DVR_TIME hknowTime;
	hknowTime.dwYear =startTime.date().year();
	hknowTime.dwMonth = startTime.date().month();
	hknowTime.dwDay = startTime.date().day();
	hknowTime.dwHour = startTime.time().hour();
	hknowTime.dwMinute = startTime.time().minute();
	hknowTime.dwSecond =startTime.time().second();
	if (strncmp(DeviceType,"DH",2)==0)
	{
		if (!CLIENT_SeekPlayBack(m_lhPlayBack,addSec,0))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("调整回放进度失败"));
			error = CLIENT_GetLastError() & 0x7fffffff;
		}
	} 
	else
	{
		if (!NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYSETTIME, &(hknowTime), sizeof(NET_DVR_TIME), NULL, NULL))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("调整回放进度失败"));
			error = NET_DVR_GetLastError();
		}
	}
}
void MemroyPlay::setvideoPos()
{
	float videosize = videoControl->value();
	unsigned int setvs = (videosize / 1000) * 65535;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		if (!CLIENT_OpenSound(m_lhPlayBack))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("打开声音失败"));
			return;
		}
		playVideo = true;
		setvs = (videosize / 1000) * 100;
		if (!CLIENT_SetVolume(m_lhPlayBack,setvs))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("设置音量失败"));
			int error = CLIENT_GetLastError() & 0x7fffffff;
		}
	} 
	else
	{
		if (!NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYAUDIOVOLUME, &setvs, 4, NULL, NULL))
		{
			QMessageBox::about(NULL, QString::fromLocal8Bit("录像控制"), QString::fromLocal8Bit("设置音量失败"));
			int error = NET_DVR_GetLastError();
		}
	}
}
void MemroyPlay::parentshow()
{
	parentshow_s();
}
void MemroyPlay::Timefunc()
{
	if (m_lhPlayBack==-1)
	{
		return;
	}
	NET_DVR_TIME nowtime;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		NET_TIME lpOSDTime;
		NET_TIME lpStartTime;
		NET_TIME lpEndTime;
		if (CLIENT_GetPlayBackOsdTime(m_lhPlayBack,&lpOSDTime,&lpStartTime,&lpEndTime))
		{
			QString strStartTime = QString("%1-%2-%3 %4:%5:%6").arg(lpStartTime.dwYear).arg(lpStartTime.dwMonth, 2, 10, QChar('0')).arg(lpStartTime.dwDay, 2, 10, QChar('0')).arg(lpStartTime.dwHour, 2, 10, QChar('0')).arg(lpStartTime.dwMinute, 2, 10, QChar('0')).arg(lpStartTime.dwSecond, 2, 10, QChar('0'));
			QString strEndTime = QString("%1-%2-%3 %4:%5:%6").arg(lpEndTime.dwYear).arg(lpEndTime.dwMonth, 2, 10, QChar('0')).arg(lpEndTime.dwDay, 2, 10, QChar('0')).arg(lpEndTime.dwHour, 2, 10, QChar('0')).arg(lpEndTime.dwMinute, 2, 10, QChar('0')).arg(lpEndTime.dwSecond, 2, 10, QChar('0'));
			QString nowTime = QString("%1-%2-%3 %4:%5:%6").arg(lpOSDTime.dwYear).arg(lpOSDTime.dwMonth, 2, 10, QChar('0')).arg(lpOSDTime.dwDay, 2, 10, QChar('0')).arg(lpOSDTime.dwHour, 2, 10, QChar('0')).arg(lpOSDTime.dwMinute, 2, 10, QChar('0')).arg(lpOSDTime.dwSecond, 2, 10, QChar('0'));
			QDateTime startTime = QDateTime::fromString(strStartTime, "yyyy-MM-dd hh:mm:ss");
			QDateTime endTime = QDateTime::fromString(strEndTime, "yyyy-MM-dd hh:mm:ss");
			QDateTime NowTime = QDateTime::fromString(nowTime, "yyyy-MM-dd hh:mm:ss");
			//总秒数
			UINT64 totalSec = startTime.secsTo(endTime);
			UINT64 nowSec = startTime.secsTo(NowTime);
			int nowValue = (float)nowSec / (float)totalSec * 1000;
			playbackSlider->setValue(nowValue);
		}

	} 
	else
	{
		if (NET_DVR_GetPlayBackOsdTime(m_lhPlayBack, &nowtime))
		{
			QString strStartTime = QString("%1-%2-%3 %4:%5:%6").arg(sTime->date().year()).arg(sTime->date().month(), 2, 10, QChar('0')).arg(sTime->date().day(), 2, 10, QChar('0')).arg(sTime->time().hour(), 2, 10, QChar('0')).arg(sTime->time().minute(), 2, 10, QChar('0')).arg(sTime->time().second(), 2, 10, QChar('0'));
			QString strEndTime = QString("%1-%2-%3 %4:%5:%6").arg(eTime->date().year()).arg(eTime->date().month(), 2, 10, QChar('0')).arg(eTime->date().day(), 2, 10, QChar('0')).arg(eTime->time().hour(), 2, 10, QChar('0')).arg(eTime->time().minute(), 2, 10, QChar('0')).arg(eTime->time().second(), 2, 10, QChar('0'));
			QString nowTime = QString("%1-%2-%3 %4:%5:%6").arg(nowtime.dwYear).arg(nowtime.dwMonth, 2, 10, QChar('0')).arg(nowtime.dwDay, 2, 10, QChar('0')).arg(nowtime.dwHour, 2, 10, QChar('0')).arg(nowtime.dwMinute, 2, 10, QChar('0')).arg(nowtime.dwSecond, 2, 10, QChar('0'));
			QDateTime startTime = QDateTime::fromString(strStartTime, "yyyy-MM-dd hh:mm:ss");
			QDateTime endTime = QDateTime::fromString(strEndTime, "yyyy-MM-dd hh:mm:ss");
			QDateTime NowTime = QDateTime::fromString(nowTime, "yyyy-MM-dd hh:mm:ss");
			//总秒数
			UINT64 totalSec = startTime.secsTo(endTime);
			UINT64 nowSec = startTime.secsTo(NowTime);
			int nowValue = (float)nowSec / (float)totalSec * 1000;
			playbackSlider->setValue(nowValue);
		}
	}
}
void MemroyPlay::seekFunc()
{
	fileWidget->clear();
	if (strncmp(DeviceType,"DH",2)==0)
	{
		NET_TIME time_start;
		NET_TIME time_end;
		time_start.dwYear = sTime->date().year();
		time_start.dwMonth = sTime->date().month();
		time_start.dwDay = sTime->date().day();
		time_start.dwHour = sTime->time().hour();
		time_start.dwMinute = sTime->time().minute();
		time_start.dwSecond = sTime->time().second();
		time_end.dwYear = eTime->date().year();
		time_end.dwMonth = eTime->date().month();
		time_end.dwDay = eTime->date().day();
		time_end.dwHour = eTime->time().hour();
		time_end.dwMinute = eTime->time().minute();
		time_end.dwSecond = eTime->time().second();
		char Recordinfo[1024 * 10];
		LONG id_find = CLIENT_FindFile(logID, ipc->currentText().mid(2).toInt()-1, 0, NULL, &time_start, &time_end, false, 2000);
		if(id_find!=0)
		{
			bool loop = true;
			while (loop)
			{
				NET_RECORDFILE_INFO findData;
				int ret = CLIENT_FindNextFile(id_find,&findData);
				QTreeWidgetItem* addItem;
				QString filename;
				QString dhfilename;
				switch (ret)
				{
				case 1:
					filename = QString::fromLocal8Bit("通道%1 %2-%3-%4 %5:%6:%7 %8-%9-%10 %11:%12:%13").arg(findData.ch+1).arg(findData.starttime.dwYear).arg(findData.starttime.dwMonth, 2, 10, QChar('0'))
						.arg(findData.starttime.dwDay, 2, 10, QChar('0')).arg(findData.starttime.dwHour, 2, 10, QChar('0')).arg(findData.starttime.dwMinute, 2, 10, QChar('0')).arg(findData.starttime.dwSecond, 2, 10, QChar('0'))
						.arg(findData.endtime.dwYear).arg(findData.endtime.dwMonth, 2, 10, QChar('0')).arg(findData.endtime.dwDay, 2, 10, QChar('0')).arg(findData.endtime.dwHour, 2, 10, QChar('0')) \
						.arg(findData.endtime.dwMinute, 2, 10, QChar('0')).arg(findData.endtime.dwSecond, 2, 10, QChar('0'));
					addItem = new QTreeWidgetItem(fileWidget);
					addItem->setText(0, filename);
					memset(Recordinfo, 0, 1024 * 10);
					dhfilename = QString::fromLocal8Bit("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20,%21,%22").arg(findData.ch).arg(findData.filename).arg(findData.framenum).arg(findData.size).arg(findData.starttime.dwYear).arg(findData.starttime.dwMonth)
						.arg(findData.starttime.dwDay).arg(findData.starttime.dwHour).arg(findData.starttime.dwMinute).arg(findData.starttime.dwSecond).arg(findData.endtime.dwYear).arg(findData.endtime.dwMonth).arg(findData.endtime.dwDay).arg(findData.endtime.dwHour)
						.arg(findData.endtime.dwMinute).arg(findData.endtime.dwSecond).arg(findData.driveno).arg(findData.startcluster).arg(findData.nRecordFileType).arg(findData.bImportantRecID).arg(findData.bHint).arg(findData.bRecType);
					addItem->setText(1, (dhfilename));
					memset(&findData, 0, sizeof(NET_RECORDFILE_INFO));
					break;
				case 0:
					loop = false;
					break;
				case -1:
					continue;
				}
			}
		}
	} 
	else
	{
		NET_DVR_FILECOND_V40 FileCond;
		FileCond.lChannel = ipc->currentText().right(1).toInt();
		FileCond.dwFileType = 0xff;
		FileCond.dwIsLocked = 0xff;
		FileCond.dwUseCardNo = 0;
		FileCond.struStartTime.dwYear = sTime->date().year();
		FileCond.struStartTime.dwMonth = sTime->date().month();
		FileCond.struStartTime.dwDay = sTime->date().day();
		FileCond.struStartTime.dwHour = sTime->time().hour();
		FileCond.struStartTime.dwMinute = sTime->time().minute();
		FileCond.struStartTime.dwSecond = sTime->time().second();
		FileCond.struStopTime.dwYear = eTime->date().year();
		FileCond.struStopTime.dwMonth = eTime->date().month();
		FileCond.struStopTime.dwDay = eTime->date().day();
		FileCond.struStopTime.dwHour = eTime->time().hour();
		FileCond.struStopTime.dwMinute = eTime->time().minute();
		FileCond.struStopTime.dwSecond = eTime->time().second();
		FileCond.byDrawFrame = 0;
		FileCond.byFindType = 0;
		FileCond.byQuickSearch = 0;
		FileCond.bySpecialFindInfoType = 0;
		FileCond.byStreamType = 0;
		FileCond.byAudioFile = 0;
		LONG fileID = NET_DVR_FindFile_V40(logID, &FileCond);
		if (fileID != -1)
		{
			bool loop = true;
			while (loop)
			{
				NET_DVR_FINDDATA_V40 findData;
				LONG ret = NET_DVR_FindNextFile_V40(fileID, &findData);
				QTreeWidgetItem* addItem;
				QString filename;
				switch (ret)
				{
				case NET_DVR_FILE_SUCCESS:
					filename = QString::fromLocal8Bit("通道%1 %2-%3-%4 %5:%6:%7 %8-%9-%10 %11:%12:%13").arg(FileCond.lChannel).arg(findData.struStartTime.dwYear).arg(findData.struStartTime.dwMonth, 2, 10, QChar('0'))
						.arg(findData.struStartTime.dwDay, 2, 10, QChar('0')).arg(findData.struStartTime.dwHour, 2, 10, QChar('0')).arg(findData.struStartTime.dwMinute, 2, 10, QChar('0')).arg(findData.struStartTime.dwSecond, 2, 10, QChar('0'))
						.arg(findData.struStopTime.dwYear).arg(findData.struStopTime.dwMonth, 2, 10, QChar('0')).arg(findData.struStopTime.dwDay, 2, 10, QChar('0')).arg(findData.struStopTime.dwHour, 2, 10, QChar('0')) \
						.arg(findData.struStopTime.dwMinute, 2, 10, QChar('0')).arg(findData.struStopTime.dwSecond, 2, 10, QChar('0'));
					addItem = new QTreeWidgetItem(fileWidget);
					addItem->setText(0, filename);
					addItem->setText(1, QString::fromLocal8Bit(findData.sFileName));
					memset(&findData, 0, sizeof(NET_DVR_FINDDATA_V40));
					break;
				case NET_DVR_FILE_NOFIND:
					loop = false;
					break;
				case NET_DVR_ISFINDING:
					continue;
				case NET_DVR_NOMOREFILE:
					loop = false;
					break;
				case NET_DVR_FILE_EXCEPTION:
					loop = false;
					break;
				}
			}
		}
	}
}
void MemroyPlay::DoubleClickedFunc(QTreeWidgetItem* item, int column)
{
	/*if (m_lhPlayBack != -1)
	{
		if (strncmp(DeviceType,"DH",2)==0&&m_lhPlayBack!=0)
		{
			if (!CLIENT_StopPlayBack(m_lhPlayBack))
			{
				return;
			}
		}
		else
		{
			if (!NET_DVR_StopPlayBack(m_lhPlayBack))
			{
				return;
			}
		}
		m_lhPlayBack = -1;
	}*/
	VideoWidget->show();
	int error;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		NET_RECORDFILE_INFO lpRecordFile;
		QString structQString = item->text(1);
		lpRecordFile.ch = structQString.section(',', 0, 0).toInt();
		strncpy((char*)&lpRecordFile.filename, structQString.section(',',1,1).toLocal8Bit().data(), 128);
		lpRecordFile.framenum = structQString.section(',', 2, 2).toInt();
		lpRecordFile.size = structQString.section(',', 3, 3).toInt();
		lpRecordFile.starttime.dwYear = structQString.section(',',4, 4).toInt();
		lpRecordFile.starttime.dwMonth = structQString.section(',', 5, 5).toInt();
		lpRecordFile.starttime.dwDay = structQString.section(',', 6, 6).toInt();
		lpRecordFile.starttime.dwHour = structQString.section(',', 7, 7).toInt();
		lpRecordFile.starttime.dwMinute = structQString.section(',', 8, 8).toInt();
		lpRecordFile.starttime.dwSecond = structQString.section(',', 9, 9).toInt();
		lpRecordFile.endtime.dwYear = structQString.section(',', 10, 10).toInt();
		lpRecordFile.endtime.dwMonth = structQString.section(',', 11, 11).toInt();
		lpRecordFile.endtime.dwDay = structQString.section(',', 12, 12).toInt();
		lpRecordFile.endtime.dwHour = structQString.section(',', 13, 13).toInt();
		lpRecordFile.endtime.dwMinute = structQString.section(',', 14, 14).toInt();
		lpRecordFile.endtime.dwSecond = structQString.section(',', 15, 15).toInt();
		lpRecordFile.driveno = structQString.section(',', 16, 16).toInt();
		lpRecordFile.startcluster = structQString.section(',', 17, 17).toInt();
		lpRecordFile.nRecordFileType = *structQString.section(',', 18, 18).toLocal8Bit().data();
		lpRecordFile.bImportantRecID = *structQString.section(',', 19, 19).toLocal8Bit().data();
		lpRecordFile.bHint = *structQString.section(',', 20, 20).toLatin1().data();
		lpRecordFile.bRecType = *structQString.section(',', 21, 21).toLatin1().data();
		m_lhPlayBack = CLIENT_PlayBackByRecordFile(logID, &lpRecordFile, (HWND)VideoWidget->winId(), NULL, 0);
		if (m_lhPlayBack != 0)
		{
			playbackSlider->setPlayback(m_lhPlayBack);
			TimeSetting->start(2000);
			pauseButton->setEnabled(true);
			fastButton->setEnabled(true);
			slowButton->setEnabled(true);
			playbacktrueButton->setEnabled(true);
			//playbackfalseButton->setEnabled(true);
			voideButton->setEnabled(true);
			otherButton->setEnabled(true);
			playbackSlider->setEnabled(false);
			videoControl->setEnabled(true);
			//设置声音按钮
			playVideo = true;
			QAction* videoAction = voideButton->defaultAction();
			videoAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/voice.png"));
			voideButton->setIconSize(QSize(20, 20));
			voideButton->setDefaultAction(videoAction);
			voideButton->update();
		}
	} 
	else
	{
		m_lhPlayBack = NET_DVR_PlayBackByName(logID, item->text(1).toLocal8Bit().data(), (HWND)VideoWidget->winId());
		if (m_lhPlayBack != -1)
		{
			playbackSlider->setPlayback(m_lhPlayBack);
			if (!NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYSTART, NULL, 0, NULL, NULL))
			{
				QMessageBox::about(NULL, QString::fromLocal8Bit("录像回放"), QString::fromLocal8Bit("开启回放失败"));
			}
			if (NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYSTARTAUDIO, NULL, 0, NULL, NULL))
			{

			}
			TimeSetting->start(2000);
			pauseButton->setEnabled(true);
			fastButton->setEnabled(true);
			slowButton->setEnabled(true);
			playbacktrueButton->setEnabled(true);
			//playbackfalseButton->setEnabled(true);
			voideButton->setEnabled(true);
			otherButton->setEnabled(true);
			playbackSlider->setEnabled(false);
			videoControl->setEnabled(true);
			//设置声音按钮
			playVideo = true;
			QAction* videoAction = voideButton->defaultAction();
			videoAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/voice.png"));
			voideButton->setIconSize(QSize(20, 20));
			voideButton->setDefaultAction(videoAction);
			voideButton->update();
		}
	}
	if (m_lhPlayBack == -1)
	{
		QMessageBox::about(NULL, QString::fromLocal8Bit("录像回放"), QString::fromLocal8Bit("回放失败"));
	}
	return;
}
void MemroyPlay::setTime(const QDate &date)
{
	if (doubleTimer->isActive())
	{
		play();
		return;
	}
	doubleTimer->start(250);
	sTime->setDate(date);
	sTime->setTime(QTime(0, 0));
	eTime->setDate(date);
	eTime->setTime(QTime(23, 59));
}
void MemroyPlay::processOneThing()
{
	doubleTimer->stop();
}
void MemroyPlay::play()
{
	QString strStartTime = QString("%1-%2-%3 %4:%5:%6").arg(sTime->date().year()).arg(sTime->date().month(), 2, 10, QChar('0')).arg(sTime->date().day(), 2, 10, QChar('0')).arg(sTime->time().hour(), 2, 10, QChar('0')).arg(sTime->time().minute(), 2, 10, QChar('0')).arg(sTime->time().second(), 2, 10, QChar('0'));
	QString strEndTime = QString("%1-%2-%3 %4:%5:%6").arg(eTime->date().year()).arg(eTime->date().month(), 2, 10, QChar('0')).arg(eTime->date().day(), 2, 10, QChar('0')).arg(eTime->time().hour(), 2, 10, QChar('0')).arg(eTime->time().minute(), 2, 10, QChar('0')).arg(eTime->time().second(), 2, 10, QChar('0'));
	QDateTime startTime = QDateTime::fromString(strStartTime, "yyyy-MM-dd hh:mm:ss");
	QDateTime endTime = QDateTime::fromString(strEndTime, "yyyy-MM-dd hh:mm:ss");

	//总秒数
	UINT64 totalSec = startTime.secsTo(endTime);
	if (totalSec<=0)
	{
		QMessageBox::about(NULL, QString::fromLocal8Bit("录像回放"), QString::fromLocal8Bit("时间错误"));
		return;
	}
	VideoWidget->show();
	NET_DVR_VOD_PARA vodPara = { 0 };//录像回放结构体
	vodPara.dwSize = sizeof(vodPara);//结构体大小
	vodPara.struIDInfo.dwSize = sizeof(vodPara.struIDInfo);//流ID信息结构体的结构体大小
	vodPara.struIDInfo.dwChannel = ipc->currentText().mid(2).toInt();//关联的设备通道

	vodPara.struBeginTime.dwYear = sTime->date().year();//开始时间参数结构体
	vodPara.struBeginTime.dwMonth = sTime->date().month();
	vodPara.struBeginTime.dwDay = sTime->date().day();
	vodPara.struBeginTime.dwHour = sTime->time().hour();
	vodPara.struBeginTime.dwMinute = sTime->time().minute();
	vodPara.struBeginTime.dwSecond = sTime->time().second();

	vodPara.struEndTime.dwYear =eTime->date().year();//结束时间参数结构体
	vodPara.struEndTime.dwMonth = eTime->date().month();
	vodPara.struEndTime.dwDay = eTime->date().day();
	vodPara.struEndTime.dwHour = eTime->time().hour();
	vodPara.struEndTime.dwMinute =eTime->time().minute();
	vodPara.struEndTime.dwSecond = eTime->time().second();
	vodPara.hWnd = (HWND)VideoWidget->winId();//回放窗口句柄
	vodPara.byDrawFrame = 0;//是否抽帧：否
	vodPara.byVolumeType = 0;//0-普通录像卷，1-存档卷，适用于CVR设备，普通卷用于通道录像，存档卷用于备份录像 
	vodPara.byStreamType = 0;//码流类型：0- 主码流，1- 子码流，2- 码流三
	vodPara.byAudioFile = 0;//回放音频文件：0- 不回放音频文件，1- 回放音频文件，该功能需要设备支持，启动音频回放后只回放音频文件 
							//按流ID和时间回放录像文件 参数：NET_DVR_Login_V40等登录接口的返回值，查找条件
	/*if (m_lhPlayBack != -1)
	{
		if (strncmp(DeviceType,"DH",2)==0&&m_lhPlayBack!=0)
		{
			if (!CLIENT_StopPlayBack(m_lhPlayBack))
			{
				return;
			}
		}
		else if(m_lhPlayBack!=-1)
		{
			if (!NET_DVR_StopPlayBack(m_lhPlayBack))
			{
				return;
			}
		}
		m_lhPlayBack = -1;
	}*/
	if (strncmp(DeviceType,"DH",2)==0)
	{
		NET_TIME lpStartTime;
		NET_TIME lpEndTime;
		lpStartTime.dwYear = sTime->date().year();//开始时间参数结构体
		lpStartTime.dwMonth = sTime->date().month();
		lpStartTime.dwDay = sTime->date().day();
		lpStartTime.dwHour = sTime->time().hour();
		lpStartTime.dwMinute = sTime->time().minute();
		lpStartTime.dwSecond = sTime->time().second();

		lpEndTime.dwYear = eTime->date().year();//结束时间参数结构体
		lpEndTime.dwMonth = eTime->date().month();
		lpEndTime.dwDay = eTime->date().day();
		lpEndTime.dwHour = eTime->time().hour();
		lpEndTime.dwMinute = eTime->time().minute();
		lpEndTime.dwSecond = eTime->time().second();
		int nChannel = ipc->currentText().mid(2).toInt();
		m_lhPlayBack = CLIENT_PlayBackByTime(logID, ipc->currentText().mid(2).toInt()-1, &lpStartTime, &lpEndTime, (HWND)VideoWidget->winId(), NULL, 0);
		nChannel = CLIENT_GetLastError() & 0x7fffffff;
		if (m_lhPlayBack!=0)
		{
			playbackSlider->setPlayback(m_lhPlayBack);
			TimeSetting->start(2000);
			pauseButton->setEnabled(true);
			fastButton->setEnabled(true);
			slowButton->setEnabled(true);
			playbacktrueButton->setEnabled(true);
			//playbackfalseButton->setEnabled(true);
			voideButton->setEnabled(true);
			otherButton->setEnabled(true);
			playbackSlider->setEnabled(true);
			videoControl->setEnabled(true);
			//设置声音按钮
			playVideo = true;
			QAction* videoAction = voideButton->defaultAction();
			videoAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/voice.png"));
			voideButton->setIconSize(QSize(20, 20));
			voideButton->setDefaultAction(videoAction);
			voideButton->update();

		}
	} 
	else
	{
		m_lhPlayBack = NET_DVR_PlayBackByTime_V40(logID, &vodPara);
		int error;
		if (m_lhPlayBack != -1)
		{
			playbackSlider->setPlayback(m_lhPlayBack);
			if (!NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYSTART, NULL, 0, NULL, NULL))
			{
				QMessageBox::about(NULL, QString::fromLocal8Bit("录像回放"), QString::fromLocal8Bit("开启回放失败"));
				error = NET_DVR_GetLastError();
			}
			if (NET_DVR_PlayBackControl_V40(m_lhPlayBack, NET_DVR_PLAYSTARTAUDIO, NULL, 0, NULL, NULL))
			{

			}
			TimeSetting->start(2000);
			pauseButton->setEnabled(true);
			fastButton->setEnabled(true);
			slowButton->setEnabled(true);
			playbacktrueButton->setEnabled(true);
			//playbackfalseButton->setEnabled(true);
			voideButton->setEnabled(true);
			otherButton->setEnabled(true);
			playbackSlider->setEnabled(true);
			videoControl->setEnabled(true);
			//设置声音按钮
			playVideo = true;
			QAction* videoAction = voideButton->defaultAction();
			videoAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/voice.png"));
			voideButton->setIconSize(QSize(20, 20));
			voideButton->setDefaultAction(videoAction);
			voideButton->update();
		}
	}
	if (m_lhPlayBack==-1)
	{
		QMessageBox::about(NULL, QString::fromLocal8Bit("录像回放"), QString::fromLocal8Bit("回放失败"));
	}
}
void MemroyPlay::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastmousePosition = event->globalPos();
	}
}
void MemroyPlay::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
	{
		if (ReleaseCapture())
			SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
		event->ignore();
		//this->move(this->pos() + (event->globalPos() - mLastmousePosition));
		//mLastmousePosition = event->globalPos();
	}
}
void MemroyPlay::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
	}
}
void MemroyPlay::paintEvent(QPaintEvent *e) {
	QPainter p(this);
	//p.begin();
	p.setPen(Qt::black);
	p.drawRect(QRect(252, 52, 746, 648));
	p.drawRect(QRect(5, 52, 993, 743));
	p.end();
}
long MemroyPlay::GetIpNumbers(QStringList* &ipList)
{
	NET_DVR_IPPARACFG_V40 ipInfo;
	NET_DVR_PICCFG_V40 piccfg;
	DWORD reLength;
	char osdlist[30];
	if (strncmp(DeviceType,"DH",2)==0)
	{
		NET_DVR_IPPARACFG_V40 ipInfo;
		NET_DVR_PICCFG_V40 piccfg;
		DWORD reLength;
		if (strncmp(DeviceType, "DH", 2) == 0)
		{
			NET_IN_GET_CAMERA_STATEINFO stIn = { 0 };
			stIn.dwSize = sizeof(NET_IN_GET_CAMERA_STATEINFO);
			stIn.bGetAllFlag = true;
			NET_OUT_GET_CAMERA_STATEINFO stOut = { 0 };
			stOut.dwSize = sizeof(NET_OUT_GET_CAMERA_STATEINFO);
			stOut.nMaxNum = 128;
			stOut.pCameraStateInfo = new NET_CAMERA_STATE_INFO[stOut.nMaxNum];
			memset(stOut.pCameraStateInfo, 0, sizeof(NET_CAMERA_STATE_INFO)*(stOut.nMaxNum));
			AV_CFG_ChannelName stInfo;
			stInfo.nStructSize = sizeof(AV_CFG_ChannelName);
			char ChannelName[1024 * 20];
			int NumOfError;
			if (CLIENT_QueryDevInfo(logID, NET_QUERY_GET_CAMERA_STATE, &stIn, &stOut, 0, 2000))
			{
				for (int i = 0; i < stOut.nValidNum; i++)
				{
					if (stOut.pCameraStateInfo[i].emConnectionState == EM_CAMERA_STATE_TYPE_CONNECTED)
					{
						*ipList << QString::fromLocal8Bit("通道%1").arg(i + 1);
					}
				}
			}
			delete[] stOut.pCameraStateInfo;
		}
	} 
	else
	{
		if (NET_DVR_GetDVRConfig(logID, NET_DVR_GET_IPPARACFG_V40, 0, &ipInfo, sizeof(NET_DVR_IPPARACFG_V40), &reLength))
		{
			int factI;
			for (int i = 0, factI = 0; i < 64 && i < ipInfo.dwDChanNum; i++, factI++)
			{
				if (ipInfo.struIPDevInfo[i].byEnable)
				{
					while (!ipInfo.struStreamMode[factI].uGetStream.struChanInfo.byChannel)
					{
						factI++;
					}
					memset(osdlist, 0, 30);
					sprintf(osdlist, "通道%d", factI + 1);
					*ipList << QString::fromLocal8Bit(osdlist);
				}
			}
			if (ipInfo.dwDChanNum > 64)
			{
				memset(&ipInfo, 0, sizeof(NET_DVR_IPPARACFG_V40));
				if (NET_DVR_GetDVRConfig(logID, NET_DVR_GET_IPPARACFG_V40, 1, &ipInfo, sizeof(NET_DVR_IPPARACFG_V40), &reLength))
				{
					for (int i = 0; i < 64 && i < ipInfo.dwDChanNum - 64; i++)
					{
						if (ipInfo.struIPDevInfo[i].byEnable)
						{
							memset(osdlist, 0, 30);
							sprintf(osdlist, "通道%d", i + 1);
							*ipList << QString::fromLocal8Bit(osdlist);
						}
					}
				}
			}
		}
	}
	return 0;
}
