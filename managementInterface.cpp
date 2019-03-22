#include "managementInterface.h"
#include <QComboBox>
#include <QTimeEdit>
#include <QLabel>
#include <QToolButton>
#include <QMouseEvent>
#include <windows.h>
#include "WindowTitle.h"
#include <QAction>
#include <QGridLayout>
#include <QScrollArea>
#include "HCNetSDK.h"
#include "dhnetsdk.h"
#include "dhconfigsdk.h"
#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include <QPainter>
//#include "CustomSlider.h"
using namespace std;
#define MWIDTH 925
#define MHEIGT 895
ManagerInterface::ManagerInterface(QWidget *parent ,char* ips, char* devicetype, long loginID):ip(ips),longid(loginID)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->resize(MWIDTH, MHEIGT);
	strcpy(DeviceType, devicetype);
	/*WindowTitle* wTitle = new WindowTitle(this, MWIDTH, 60);
	wTitle->setGeometry(0, 0, MWIDTH, 60);*/
	//wTitle->show();

	QToolButton* closeButton = new QToolButton(this);
	closeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* pActions = new QAction(closeButton);
	pActions->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/close_wnd_h.png"));
	closeButton->setIconSize(QSize(50, 30));
	closeButton->setDefaultAction(pActions);
	closeButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	closeButton->setGeometry(MWIDTH-50 , 0, 50, 30);
	//closeButton->show();
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(parentshow()));
	//this->move(10, 10);
	this->setWindowTitle(QString::fromLocal8Bit(ip));
	ipcnumber = new QComboBox(this);
	QStringList* ipstring=new QStringList;
	ipstring->clear();
	GetIpNumber(ipstring);
	ipcnumber->addItems(*ipstring);

	/*QScrollArea* m_scrollArea = new QScrollArea;
	m_scrollArea->setAlignment(Qt::AlignBottom);
	m_scrollArea->setParent(this);
	m_scrollArea->setMinimumSize(1500, 700);
	m_scrollArea->show();*/

	QLabel* allStartTime[8] ;
	QLabel* ALLETime[8] ;
	QLabel* sLabel = new QLabel(this);
	sLabel->setText(QString::fromLocal8Bit("星期一"));
	QLabel* eLabel = new QLabel(this);
	eLabel->setText(QString::fromLocal8Bit("星期二"));
	QLabel* WELabel = new QLabel(this);
	WELabel->setText(QString::fromLocal8Bit("星期三"));
	QLabel* THLabel = new QLabel(this);
	THLabel->setText(QString::fromLocal8Bit("星期四"));
	QLabel* FRLabel = new QLabel(this);
	FRLabel->setText(QString::fromLocal8Bit("星期五"));
	QLabel* SALable = new QLabel(this);
	SALable->setText(QString::fromLocal8Bit("星期六"));
	QLabel* SULabel = new QLabel(this);
	SULabel->setText(QString::fromLocal8Bit("星期日"));

	QLabel* AAAAtime = new QLabel(this);
	AAAAtime->setText(QString::fromLocal8Bit("时间段"));
	QLabel* onetime = new QLabel(this);
	onetime->setText(QString::fromLocal8Bit("时间段1"));
	QLabel* twotime = new QLabel(this);
	twotime->setText(QString::fromLocal8Bit("时间段2"));
	QLabel* threetime = new QLabel(this);
	threetime->setText(QString::fromLocal8Bit("时间段3"));
	QLabel* fourtime = new QLabel(this);
	fourtime->setText(QString::fromLocal8Bit("时间段4"));
	QLabel* fivetime = new QLabel(this);
	fivetime->setText(QString::fromLocal8Bit("时间段5"));
	QLabel* sixtime = new QLabel(this);
	sixtime->setText(QString::fromLocal8Bit("时间段6"));
	QLabel* seventime = new QLabel(this);
	seventime->setText(QString::fromLocal8Bit("时间段7"));
	QLabel* eighttime = new QLabel(this);
	eighttime->setText(QString::fromLocal8Bit("时间段8"));

	QLabel* allLabel = new QLabel(this);
	allLabel->setText(QString::fromLocal8Bit("复制到"));
	allipc = new QComboBox(this);
	QStringList allchar;
	allchar << QString::fromLocal8Bit("无") << QString::fromLocal8Bit("所有通道") << *ipstring;
	allipc->addItems(allchar);
	QToolButton* video = new QToolButton(this);
	video->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* vAction = new QAction(video);
	vAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/advance.png"));
	video->setIconSize(QSize(100, 50));
	video->setDefaultAction(vAction);
	video->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");

	QGridLayout vsGridLayout(this);
	//vsGridLayout.addWidget(wTitle, 1, 0);
	vsGridLayout.addWidget(ipcnumber, 4, 1);
	if (strncmp(DeviceType,"DH",2)==0)
	{
		vsGridLayout.addWidget(sLabel, 4, 3);
		vsGridLayout.addWidget(eLabel, 4, 4);
		vsGridLayout.addWidget(WELabel, 4, 5);
		vsGridLayout.addWidget(THLabel, 4, 6);
		vsGridLayout.addWidget(FRLabel, 4, 7);
		vsGridLayout.addWidget(SALable, 4, 8);
		vsGridLayout.addWidget(SULabel, 4, 2);
	} 
	else
	{
		vsGridLayout.addWidget(sLabel, 4, 2);
		vsGridLayout.addWidget(eLabel, 4, 3);
		vsGridLayout.addWidget(WELabel, 4, 4);
		vsGridLayout.addWidget(THLabel, 4, 5);
		vsGridLayout.addWidget(FRLabel, 4, 6);
		vsGridLayout.addWidget(SALable, 4, 7);
		vsGridLayout.addWidget(SULabel, 4, 8);
	}

	vsGridLayout.addWidget(AAAAtime, 4, 0);
	vsGridLayout.addWidget(onetime, 5, 0);
	vsGridLayout.addWidget(twotime, 7, 0);
	vsGridLayout.addWidget(threetime, 9, 0);
	vsGridLayout.addWidget(fourtime, 11, 0);
	vsGridLayout.addWidget(fivetime, 13, 0);
	vsGridLayout.addWidget(sixtime, 15, 0);
	vsGridLayout.addWidget(seventime, 17, 0);
	vsGridLayout.addWidget(eighttime, 19, 0);

	vsGridLayout.addWidget(allLabel, 21, 0);
	vsGridLayout.addWidget(allipc, 21, 1,1,2);
	vsGridLayout.addWidget(video, 22, 7,1,2);
	vsGridLayout.setHorizontalSpacing(50);
	vsGridLayout.setVerticalSpacing(20);
	vsGridLayout.setContentsMargins(10, 10, 10, 10);
	vsGridLayout.setAlignment(Qt::AlignLeft| Qt::AlignBaseline);
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 8; j++) {
			sTime[i][j] = new QTimeEdit(this);
			eTime[i][j] = new QTimeEdit(this);
			sTime[i][j]->setMaximumTime(QTime(24, 00));
			eTime[i][j]->setMaximumTime(QTime(24, 00));
			vsGridLayout.addWidget(sTime[i][j], j*2+5,i+2);
			vsGridLayout.addWidget(eTime[i][j], j*2+ 6,i+2);
		}
	}
	for (int i = 0; i < 8; i++)
	{
		allStartTime[i] = new QLabel(this);
		allStartTime[i]->setText(QString::fromLocal8Bit("开始时间"));
		vsGridLayout.addWidget(allStartTime[i], 2 * i + 5,1);
		ALLETime[i] = new QLabel(this);
		ALLETime[i]->setText(QString::fromLocal8Bit("结束时间"));
		vsGridLayout.addWidget(ALLETime[i], 2 * i + 6,1);
	}
	this->setLayout(&vsGridLayout);
	//vsGridLayout.setParent(this);
	vsGridLayout.setGeometry(QRect(0, 150, 800, 800));
	connect(video, SIGNAL(clicked()), this, SLOT(setvideo()));
	connect(ipcnumber, SIGNAL(currentIndexChanged(QString)), this, SLOT(ChangedFunction(QString)));
	getvideo();
	this->show();
}
long ManagerInterface::GetIpNumber(QStringList* &ipList)
{
	NET_DVR_IPPARACFG_V40 ipInfo;
	NET_DVR_PICCFG_V40 piccfg;
	DWORD reLength;
	char osdlist[30];
	if (strncmp(DeviceType, "DH", 2) == 0)
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
			if (CLIENT_QueryDevInfo(longid, NET_QUERY_GET_CAMERA_STATE, &stIn, &stOut, 0, 2000))
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
		else
		{
			if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_IPPARACFG_V40, 0, &ipInfo, sizeof(NET_DVR_IPPARACFG_V40), &reLength))
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
					if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_IPPARACFG_V40, 1, &ipInfo, sizeof(NET_DVR_IPPARACFG_V40), &reLength))
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
}
void ManagerInterface::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastmousePosition = event->globalPos();
	}
}
void ManagerInterface::mouseMoveEvent(QMouseEvent* event)
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
void ManagerInterface::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
	}
}
void ManagerInterface::setvideo()
{
	//QMessageBox::about(NULL, QString::fromLocal8Bit("录像设置"), QString::fromLocal8Bit("时间核查开始"));
	//for (int i=0;i<7;i++)
	//{
	//	for (int j = 0; j < 8; j++)
	//	{
	//		QMessageBox::about(NULL,NULL,sTime[i][j]->text());
	//		QMessageBox::about(NULL,NULL,eTime[i][j]->text());
	//		if (sTime[i][j]->text() == QString::fromLatin1("00:00") && eTime[i][j]->text() == QString::fromLatin1("00:00"))
	//		{
	//			//qDebug() << i << "-" << j <<"-"<< 0 << endl;
	//			break;
	//		}
	//		if (sTime[i][j]->time().secsTo(eTime[i][j]->time())<0)
	//		{
	//			//qDebug() << i << "-" << j << "-" << 1 << endl;
	//			return;
	//		}
	//		if (j-1>=0&&j-1<8&&eTime[i][j-1]->text() != QString::fromLatin1("00:00"))
	//		{
	//			if (eTime[i][j - 1]->time().secsTo(sTime[i][j]->time())<0)
	//			{
	//				//qDebug() << i << "-" << j << "-" << 2 << endl;
	//				return;
	//			}
	//		}
	//		/*if (j+1>=0&&j+1<8&&sTime[i][j+1]->text()!=QString::fromLatin1("00:00"))
	//		{
	//			if (eTime[i][j]->text().toInt()>sTime[i][j+1]->text().toInt())
	//			{
	//				return;
	//			}
	//		}*/
	//	}
	//}
	//QMessageBox::about(NULL, QString::fromLocal8Bit("录像设置"), QString::fromLocal8Bit("时间核查结束"));
	QString ipcNumberofset = allipc->currentText();
	QList<int> numbreList;
	if (ipcNumberofset==QString::fromLocal8Bit("无"))
	{

	}else if (ipcNumberofset==QString::fromLocal8Bit("所有通道"))
	{
		QList<QString> StringList;
		for (int i = 0; i < ipcnumber->count(); i++)
		{
			QString itemString = ipcnumber->itemText(i);
			if (!itemString.isEmpty())
			{
				numbreList.append(itemString.mid(2).toInt());
			}
		}
	}
	else {
		numbreList.append(ipcnumber->currentText().mid(2).toInt());
		numbreList.append(allipc->currentText().mid(2).toInt());
	}
	QString numberofIPC = ipcnumber->currentText().mid(2);
	long channel = numberofIPC.toInt();
	if (channel==0)
	{
		return;
	}
	QTime stime;
	QTime etime;
	NET_DVR_RECORD_V40 ipcRecord;
	DWORD reapp;
	//QMessageBox::about(NULL, QString::fromLocal8Bit("录像设置"), QString::fromLocal8Bit("获取录像计划开始"));
	if (strncmp(DeviceType,"DH",2)==0)
	{
		DHDEV_RECORD_CFG dhipcRecord;
		CFG_RECORD_INFO  dhRecordinfo;
		char* szout = new char[1024 * 10];
		int error;
		if (CLIENT_GetNewDevConfig(longid, CFG_CMD_RECORD, channel - 1,szout, 1024*10, &error))
		{
			if (!CLIENT_ParseData(CFG_CMD_RECORD,szout,&dhRecordinfo,sizeof(CFG_RECORD_INFO),NULL))
			{
				return;
			}
			/*dhipcRecord.dwSize = sizeof(DHDEV_RECORD_CFG);
			dhipcRecord.byRecordType = 0;
			memset(&dhipcRecord.stSect, 0, sizeof(DH_TSECT)*42);*/
			dhRecordinfo.nStreamType = 0;
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					stime = sTime[i][j]->time();
					etime = eTime[i][j]->time();
					if (stime.secsTo(QTime(0, 0, 0, 0)) != 0 || etime.secsTo(QTime(0, 0, 0, 0)) != 0)
					{
						dhRecordinfo.stuTimeSection[i][j].dwRecordMask = 4;
						dhRecordinfo.stuTimeSection[i][j].nBeginHour = stime.hour();
						dhRecordinfo.stuTimeSection[i][j].nBeginMin = stime.minute();
						dhRecordinfo.stuTimeSection[i][j].nEndHour = etime.hour();
						dhRecordinfo.stuTimeSection[i][j].nEndMin = etime.minute();
						if (etime.secsTo(QTime(23, 59, 0, 0)) == 0)
						{
							dhRecordinfo.stuTimeSection[i][j].nEndHour = 24;
							dhRecordinfo.stuTimeSection[i][j].nEndMin =0;
						}
						/*dhipcRecord.stSect[i][j].bEnable = 4;
						dhipcRecord.stSect[i][j].iBeginHour = stime.hour();
						dhipcRecord.stSect[i][j].iBeginMin= stime.minute();
						dhipcRecord.stSect[i][j].iEndHour = etime.hour();
						dhipcRecord.stSect[i][j].iEndMin = etime.minute();
						if (etime.secsTo(QTime(23, 59, 0, 0)) == 0)
						{
							dhipcRecord.stSect[i][j].iEndHour = 24;
							dhipcRecord.stSect[i][j].iEndMin = 0;
						}*/
					}
				}
			}
		}
		if (numbreList.isEmpty())
		{
			/*if (!CLIENT_SetDevConfig(longid, DH_DEV_RECORDCFG, channel - 1, &dhipcRecord, sizeof(DHDEV_RECORD_CFG), 2000))
			{
				QMessageBox::about(NULL, QString::fromLocal8Bit("录像设置"), QString::fromLocal8Bit("设置失败"));
				return;
			}*/
			if (!CLIENT_PacketData(CFG_CMD_RECORD,(char*)&dhRecordinfo,sizeof(CFG_RECORD_INFO),szout,1024*10))
			{
				return;
			}
			if (!CLIENT_SetNewDevConfig(longid, CFG_CMD_RECORD, channel - 1, szout, 1024*10,&error,0,2000))
			{
				QMessageBox::about(NULL, QString::fromLocal8Bit("录像设置"), QString::fromLocal8Bit("设置失败"));
				return;
			}
		}
		else
		{
			QList<int>::Iterator ListChannel;
			for (ListChannel = numbreList.begin(); ListChannel != numbreList.end(); ListChannel++)
			{
				int a = *ListChannel - 1;
				dhRecordinfo.nChannelID = *ListChannel - 1;
				if (!CLIENT_PacketData(CFG_CMD_RECORD, (char*)&dhRecordinfo, sizeof(CFG_RECORD_INFO), szout, 1024 * 10))
				{
					return;
				}
				if (!CLIENT_SetNewDevConfig(longid, CFG_CMD_RECORD, *ListChannel - 1, szout, 1024 * 10, &error, 0, 2000))
				{
					QMessageBox::about(NULL, QString::fromLocal8Bit("录像设置"), QString::fromLocal8Bit("设置失败"));
					return;
				}
			}
		}
	} 
	else
	{
		if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_RECORDCFG_V40, channel, &ipcRecord, sizeof(NET_DVR_RECORD_V40), &reapp))
		{
			//QMessageBox::about(NULL, QString::fromLocal8Bit("录像设置"), QString::fromLocal8Bit("获取录像计划成功"));
			ipcRecord.byStreamType = 0;
			ipcRecord.dwRecord = 1;
			ipcRecord.wLockDuration = 0;
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					stime = sTime[i][j]->time();
					etime = eTime[i][j]->time();
					if (stime.secsTo(QTime(0, 0, 0, 0)) == 0 && etime.secsTo(QTime(23, 59, 0, 0)) == 0)
					{
						ipcRecord.struRecAllDay[i].byAllDayRecord = 1;
						ipcRecord.struRecAllDay[i].byRecordType = 0;
					}
					else {
						ipcRecord.struRecordSched[i][j].byRecordType = 0;
						ipcRecord.struRecordSched[i][j].struRecordTime.byStartHour = stime.hour();
						ipcRecord.struRecordSched[i][j].struRecordTime.byStartMin = stime.minute();
						ipcRecord.struRecordSched[i][j].struRecordTime.byStopHour = etime.hour();
						ipcRecord.struRecordSched[i][j].struRecordTime.byStopMin = etime.minute();
						if (etime.secsTo(QTime(23, 59, 0, 0)) == 0)
						{
							ipcRecord.struRecordSched[i][j].struRecordTime.byStopHour = 24;
							ipcRecord.struRecordSched[i][j].struRecordTime.byStopMin = 0;
						}
					}
				}
			}
			if (numbreList.isEmpty())
			{
				if (!NET_DVR_SetDVRConfig(longid, NET_DVR_SET_RECORDCFG_V40, channel, &ipcRecord, sizeof(NET_DVR_RECORD_V40)))
				{
					QMessageBox::about(NULL, QString::fromLocal8Bit("录像设置"), QString::fromLocal8Bit("设置失败"));
					return;
				}
			}
			else
			{
				QList<int>::Iterator ListChannel;
				for (ListChannel = numbreList.begin(); ListChannel != numbreList.end(); ListChannel++)
				{
					if (!NET_DVR_SetDVRConfig(longid, NET_DVR_SET_RECORDCFG_V40, *ListChannel, &ipcRecord, sizeof(NET_DVR_RECORD_V40)))
					{
						QMessageBox::about(NULL, QString::fromLocal8Bit("录像设置"), QString::fromLocal8Bit("设置失败"));
						return;
					}
				}
			}

		}
	}
	QMessageBox::about(NULL, QString::fromLocal8Bit("录像设置"), QString::fromLocal8Bit("设置完成"));
}
void ManagerInterface::getvideo(QString ipcnum)
{
	QString currentNumber;
	if (ipcnum.isEmpty())
	{
		currentNumber = ipcnumber->currentText().mid(2);
	}else {
		currentNumber = ipcnum.right(1);
	}
	long channel = currentNumber.toLong();
	NET_DVR_RECORD_V40 ipcRecord;
	DWORD reapp;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		DHDEV_RECORD_CFG dhipcRecord;
		if (CLIENT_GetDevConfig(longid, DH_DEV_RECORDCFG, channel - 1, &dhipcRecord, sizeof(DHDEV_RECORD_CFG), &reapp))
		{
			if (1)//ipcRecord.dwRecord==1)
			{
				int TimeInt[4];
				for (int i = 0; i < 7; i++)
				{
					memset(TimeInt, 0, sizeof(int) * 4);
					for (int j = 0; j < 6; j++)
					{
						sTime[i][j]->setTime(QTime(0, 0));
						eTime[i][j]->setTime(QTime(0, 0));
						if (dhipcRecord.stSect[i][j].bEnable != 0)
						{
							TimeInt[0] = (int)dhipcRecord.stSect[i][j].iBeginHour;
							TimeInt[1] = (int)dhipcRecord.stSect[i][j].iBeginMin;
							TimeInt[2] = (int)dhipcRecord.stSect[i][j].iEndHour;
							TimeInt[3] = (int)dhipcRecord.stSect[i][j].iEndMin;
							if (TimeInt[0] != 0 || TimeInt[1] != 0)
							{
								sTime[i][j]->setTime(QTime(TimeInt[0], TimeInt[1]));
							}
							if (TimeInt[2] != 0 || TimeInt[3] != 0)
							{
								eTime[i][j]->setTime(QTime(TimeInt[2], TimeInt[3]));
							}
							if (TimeInt[2] == 24 && TimeInt[3] == 0)
							{
								eTime[i][j]->setTime(QTime(23, 59));
							}
							memset(TimeInt, 0, sizeof(int) * 4);
						}
					}
				}
			}
		}
	} 
	else
	{
		if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_RECORDCFG_V40, channel, &ipcRecord, sizeof(NET_DVR_RECORD_V40), &reapp))
		{
			if (1)//ipcRecord.dwRecord==1)
			{
				int TimeInt[4];
				for (int i = 0; i < 7; i++)
				{
					memset(TimeInt, 0, sizeof(int) * 4);
					for (int j = 0; j < 6; j++)
					{
						sTime[i][j]->setTime(QTime(0, 0));
						eTime[i][j]->setTime(QTime(0, 0));
					}
					if (ipcRecord.struRecAllDay[i].byAllDayRecord&&ipcRecord.struRecAllDay[i].byRecordType == 0)
					{
						sTime[i][0]->setTime(QTime(0, 0));
						eTime[i][0]->setTime(QTime(23, 59));
						continue;
					}
					for (int j = 0; j < 8; j++)
					{
						if (ipcRecord.struRecordSched[i][j].byRecordType == 0)
						{
							TimeInt[0] = (int)ipcRecord.struRecordSched[i][j].struRecordTime.byStartHour;
							TimeInt[1] = (int)ipcRecord.struRecordSched[i][j].struRecordTime.byStartMin;
							TimeInt[2] = (int)ipcRecord.struRecordSched[i][j].struRecordTime.byStopHour;
							TimeInt[3] = (int)ipcRecord.struRecordSched[i][j].struRecordTime.byStopMin;
							if (TimeInt[0] != 0 || TimeInt[1] != 0)
							{
								sTime[i][j]->setTime(QTime(TimeInt[0], TimeInt[1]));
							}
							if (TimeInt[2] != 0 || TimeInt[3] != 0)
							{
								eTime[i][j]->setTime(QTime(TimeInt[2], TimeInt[3]));
							}
							if (TimeInt[2] == 24 && TimeInt[3] == 0)
							{
								eTime[i][j]->setTime(QTime(23, 59));
							}
							memset(TimeInt, 0, sizeof(int) * 4);
						}
					}
				}
			}
		}
	}
	//QMessageBox::about(NULL, QString::fromLocal8Bit("录像获取"), QString::fromLocal8Bit("获取完成"));
}
void ManagerInterface::ChangedFunction(QString text)
{
	getvideo(text);
}
void ManagerInterface::parentshow()
{
	parentshow_s();
}
void ManagerInterface::paintEvent(QPaintEvent *e) {
	QPainter p(this);
	//p.begin();
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setPen(QPen((Qt::black),1));
	p.drawLine(0,160, MWIDTH,160);
	p.drawLine(0, 240, MWIDTH, 240);
	p.drawLine(0, 320, MWIDTH, 320);
	p.drawLine(0, 400, MWIDTH, 400);
	p.drawLine(0, 480, MWIDTH, 480);
	p.drawLine(0, 560, MWIDTH, 560);
	p.drawLine(0, 640, MWIDTH, 640);
	p.drawLine(0, 720, MWIDTH, 720);
	p.drawRect(QRect(1, 1, MWIDTH -3, MHEIGT -3));
	p.end();
}