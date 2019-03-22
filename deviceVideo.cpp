#include "deviceVideo.h"
#include <QTreeWidget>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <Windows.h>
#include <QMouseEvent>
#include <QAction>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPainter>
#include <QTreeWidgetItem>
#include "WindowTitle.h"
#include "login.h"
#include "HCNetSDK.h"
#include "dhnetsdk.h"
#include "dhconfigsdk.h"
#define DWINDTH 410
#define DHEIGT 405
DeviceVideo::DeviceVideo(QWidget *parent ,char* ipstring, /*char* user = NULL, char* passwd = NULL, */char* deviceType, long loginID):ip(ipstring),longid(loginID)
{
	this->setWindowTitle(QString::fromLocal8Bit(ipstring));
	this->setWindowFlags(Qt::FramelessWindowHint);
	strcpy(DeviceType,deviceType);
	/*strcpy(User, user);
	strcpy(Passwd, passwd);*/
	/*WindowTitle* wTitle = new WindowTitle(this, DWINDTH, 50);
	wTitle->setGeometry(0, 0, DWINDTH, 50);
	wTitle->show();*/
	QToolButton* closeButton = new QToolButton(this);
	closeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* pActions = new QAction(closeButton);
	pActions->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/close_wnd_h.png"));
	closeButton->setIconSize(QSize(100, 50));
	closeButton->setDefaultAction(pActions);
	closeButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	closeButton->setGeometry(DWINDTH - 50, 0, 50, 30);
	closeButton->show();
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(parentshow()));
	this->resize(DWINDTH, DHEIGT);
	ipcInface = new QTreeWidget(this);
	ipcInface->setGeometry(2, 50, 400, 300);
	ipcInface->setColumnCount(2);
	QStringList sclist;
	sclist << QString::fromLocal8Bit("IPC") << QString::fromLocal8Bit("通道标签");
	ipcInface->setHeaderLabels(sclist);
	OSDFunc();
	QToolButton* addPush =new QToolButton(this);
	addPush->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* addAction = new QAction(addPush);
	addAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/advance.png"));
	addPush->setIconSize(QSize(80, 30));
	addPush->setDefaultAction(addAction);
	addPush->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	addPush->show();
	QToolButton* deletePush = new QToolButton(this);
	deletePush->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* deleteAction = new QAction(deletePush);
	deleteAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/cancel.png"));
	deletePush->setIconSize(QSize(80, 30));
	deletePush->setDefaultAction(deleteAction);
	deletePush->setStyleSheet("color:rgb(52,52,52);background-color:transparent;");
	deletePush->show();
	addPush->setGeometry(250, 360, 80, 30);
	deletePush->setGeometry(320, 360, 80, 30);
	this->show();
	row = NULL;
	connect(ipcInface, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(doubleFunc(QTreeWidgetItem*, int)));
	connect(ipcInface, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(CloseFunc(QTreeWidgetItem*, int)));
	connect(ipcInface, SIGNAL(currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)), this, SLOT(currentItemChangedFunc(QTreeWidgetItem*, QTreeWidgetItem*)));
	connect(addPush, SIGNAL(clicked()), this, SLOT(addslot()));
	connect(deletePush, SIGNAL(clicked()), this, SLOT(deleteslot()));
}
void DeviceVideo::addslot()
{
	CloseFunc_hand();
	QTreeWidgetItemIterator it(ipcInface, QTreeWidgetItemIterator::All);
	QMap<QString, QString>::const_iterator item=OSDmap.constBegin();
	NET_DVR_PICCFG_V40 piccfg;
	DWORD reLength;
	while (*it)//&&item!=OSDmap.constEnd())
	{
		for (item=OSDmap.constBegin(); item != OSDmap.constEnd(); ++item)
		{
			if ((*it)->text(0) == item.key() && (*it)->text(1) != item.value())
			{
				long channel = item.key().mid(2).toLong();
				if (strncmp(DeviceType, "DH", 2) == 0)
				{
					DH_IN_MATRIX_GET_CAMERAS dhInMatrix = { 0 };
					dhInMatrix.dwSize = sizeof(DH_IN_MATRIX_GET_CAMERAS);
					DH_OUT_MATRIX_GET_CAMERAS dhOutMatrix = { 0 };
					dhOutMatrix.dwSize = sizeof(DH_OUT_MATRIX_GET_CAMERAS);
					dhOutMatrix.nMaxCameraCount = 128;
					dhOutMatrix.pstuCameras = new DH_MATRIX_CAMERA_INFO[dhOutMatrix.nMaxCameraCount];
					memset(dhOutMatrix.pstuCameras, 0, sizeof(DH_MATRIX_CAMERA_INFO)*(dhOutMatrix.nMaxCameraCount));
					bool a = FALSE;
					for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
					{
						dhOutMatrix.pstuCameras[i].dwSize = sizeof(DH_MATRIX_CAMERA_INFO);
						dhOutMatrix.pstuCameras[i].stuRemoteDevice.dwSize = sizeof(DH_REMOTE_DEVICE);
						dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount = 128;
						dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs = new DH_VIDEO_INPUTS[dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount];
						memset(dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs, 0, sizeof(DH_VIDEO_INPUTS)*(dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount));
						dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs->dwSize = sizeof(DH_VIDEO_INPUTS);
					}
					bool ret = CLIENT_MatrixGetCameras(longid, &dhInMatrix, &dhOutMatrix, 5000);
					if (ret)
					{
							NET_DVR_USER_LOGIN_INFO hkLoginInfo = { 0 };
							NET_DVR_DEVICEINFO_V40 hkDeviceInfo = { 0 };
							strncpy(hkLoginInfo.sDeviceAddress, dhOutMatrix.pstuCameras[channel-1].stuRemoteDevice.szIp, 64);
							strncpy(hkLoginInfo.sUserName, dhOutMatrix.pstuCameras[channel-1].stuRemoteDevice.szUserEx, 64);
							strncpy(hkLoginInfo.sPassword, dhOutMatrix.pstuCameras[channel-1].stuRemoteDevice.szPwdEx, 64);
							hkLoginInfo.wPort = 8000;
							hkLoginInfo.bUseAsynLogin = 0;
							hkLoginInfo.byJiaFaEncypt = 0;
							long ipcid = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
							if (ipcid == -1)
							{
								hkLoginInfo.byJiaFaEncypt = 1;
								ipcid = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
								if (ipcid==-1)
								{
									hkLoginInfo.byJiaFaEncypt = 2;
									ipcid = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
								}
							}
							if (ipcid != -1)
							{
								if (NET_DVR_GetDVRConfig(ipcid, NET_DVR_GET_PICCFG_V40, 1, &piccfg, sizeof(NET_DVR_PICCFG_V40), &reLength))
								{
									char ssName[50];
									memset(ssName, 0, 50);
									strncpy(ssName, (const char*)(*it)->text(1).toLocal8Bit().data(), 33);
									strncpy((char*)piccfg.sChanName, (const char*)ssName, 32);
									piccfg.dwShowChanName = 1;
									if (NET_DVR_SetDVRConfig(ipcid, NET_DVR_SET_PICCFG_V40, 1, &piccfg, sizeof(NET_DVR_PICCFG_V40)))
									{
										//OSDmap[item.key()] = (*it)->text(1);
									}
								}
							}
					}
					for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
					{
						delete[] dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs;
					}
					delete[] dhOutMatrix.pstuCameras;
				}
				else
				{
					if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_PICCFG_V40, channel, &piccfg, sizeof(NET_DVR_PICCFG_V40), &reLength))
					{
						char ssName[50];
						memset(ssName, 0, 50);
						strncpy(ssName, (const char*)(*it)->text(1).toLocal8Bit().data(), 33);
						strncpy((char*)piccfg.sChanName, (const char*)ssName, 32);
						piccfg.dwShowChanName = 1;
						if (NET_DVR_SetDVRConfig(longid, NET_DVR_SET_PICCFG_V40, channel, &piccfg, sizeof(NET_DVR_PICCFG_V40)))
						{
							//OSDmap[item.key()] = (*it)->text(1);
						}
					}
				}
				reLength = NET_DVR_GetLastError();
				break;
			}
		}
		++it;
	}
	QMessageBox::about(NULL, QString::fromLocal8Bit("OSD提示"), QString::fromLocal8Bit("修改OSD完成"));
	OSDFunc();
}
void DeviceVideo::deleteslot()
{
	this->hide();
	parentshow_s();
}
void DeviceVideo::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastmousePosition = event->globalPos();
	}
}
void DeviceVideo::mouseMoveEvent(QMouseEvent* event)
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
void DeviceVideo::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
	}
}
void DeviceVideo::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.drawRect(QRect(1, 1, DWINDTH - 3, DHEIGT - 3));
}
void DeviceVideo::doubleFunc(QTreeWidgetItem* item, int column)
{
	
	if (column == 1)
	{
		ipcInface->openPersistentEditor(item, column);
		/*if (row)
		{
			ipcInface->closePersistentEditor(row, 1);
			row = NULL;
		}*/
		row = item;
	}
}
void DeviceVideo::CloseFunc(QTreeWidgetItem* item, int column)
{
	//ipcInface->closePersistentEditor(item, 1);
}
void DeviceVideo::currentItemChangedFunc(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	ipcInface->update();
}
void DeviceVideo::CloseFunc_hand()
{
	ipcInface->closePersistentEditor(row, 1);
}
long DeviceVideo::OSDFunc()
{
	NET_DVR_IPPARACFG_V40 ipInfo;
	NET_DVR_PICCFG_V40 piccfg;
	DWORD reLength;
	OSDmap.clear();
	ipcInface->clear();
	if (strncmp(DeviceType,"DH",2)==0)
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
		DH_IN_MATRIX_GET_CAMERAS dhInMatrix = { 0 };
		dhInMatrix.dwSize = sizeof(DH_IN_MATRIX_GET_CAMERAS);
		DH_OUT_MATRIX_GET_CAMERAS dhOutMatrix = { 0 };
		dhOutMatrix.dwSize = sizeof(DH_OUT_MATRIX_GET_CAMERAS);
		dhOutMatrix.nMaxCameraCount = 128;
		dhOutMatrix.pstuCameras = new DH_MATRIX_CAMERA_INFO[dhOutMatrix.nMaxCameraCount];
		memset(dhOutMatrix.pstuCameras, 0, sizeof(DH_MATRIX_CAMERA_INFO)*(dhOutMatrix.nMaxCameraCount));
		bool a = FALSE;
		for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
		{
			dhOutMatrix.pstuCameras[i].dwSize = sizeof(DH_MATRIX_CAMERA_INFO);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.dwSize = sizeof(DH_REMOTE_DEVICE);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount = 128;
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs = new DH_VIDEO_INPUTS[dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount];
			memset(dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs, 0, sizeof(DH_VIDEO_INPUTS)*(dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount));
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs->dwSize = sizeof(DH_VIDEO_INPUTS);
		}
		bool ret = CLIENT_MatrixGetCameras(longid, &dhInMatrix, &dhOutMatrix, 5000);
		if (CLIENT_QueryDevInfo(longid, NET_QUERY_GET_CAMERA_STATE, &stIn, &stOut, 0, 2000))
		{
			for (int i = 0; i < stOut.nValidNum; i++)
			{
				if (stOut.pCameraStateInfo[i].emConnectionState == EM_CAMERA_STATE_TYPE_CONNECTED)
				{
					QTreeWidgetItem* dh_subMenu = new QTreeWidgetItem(ipcInface);
					dh_subMenu->setText(0, QString::fromLocal8Bit("通道%1").arg(i + 1));
					if (ret)
					{
							NET_DVR_USER_LOGIN_INFO hkLoginInfo = { 0 };
							NET_DVR_DEVICEINFO_V40 hkDeviceInfo = { 0 };
							strncpy(hkLoginInfo.sDeviceAddress, dhOutMatrix.pstuCameras[i].stuRemoteDevice.szIp, 64);
							strncpy(hkLoginInfo.sUserName, dhOutMatrix.pstuCameras[i].stuRemoteDevice.szUserEx, 64);
							strncpy(hkLoginInfo.sPassword, dhOutMatrix.pstuCameras[i].stuRemoteDevice.szPwdEx, 64);
							hkLoginInfo.wPort = 8000;
							hkLoginInfo.bUseAsynLogin = 0;
							hkLoginInfo.byJiaFaEncypt = 0;
							long ipcid = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
							NumOfError = NET_DVR_GetLastError();
							if (ipcid==-1)
							{
								hkLoginInfo.byJiaFaEncypt = 1;
								ipcid = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
								NumOfError = NET_DVR_GetLastError();
								if (ipcid=-1)
								{
									hkLoginInfo.byJiaFaEncypt = 2;
									ipcid = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
								}
							}
							if (ipcid != -1)
							{
								if (NET_DVR_GetDVRConfig(ipcid, NET_DVR_GET_PICCFG_V40, 1, &piccfg, sizeof(NET_DVR_PICCFG_V40), &reLength))
								{
									dh_subMenu->setText(1, QString::fromLocal8Bit((const char*)piccfg.sChanName));
									//subMenu->setTextAlignment(1, 63);
									OSDmap.insert(QString::fromLocal8Bit("通道%1").arg(i + 1), QString::fromLocal8Bit((const char*)piccfg.sChanName));
								}
							}
					}
				/*	bool ret = CLIENT_GetNewDevConfig(longid, CFG_CMD_CHANNELTITLE, i, ChannelName, 1024 * 20, &NumOfError, 2000);
					if (ret)
					{
						if (CLIENT_ParseData(CFG_CMD_CHANNELTITLE, ChannelName, &stInfo, sizeof(AV_CFG_ChannelName), &NumOfError))
						{
							dh_subMenu->setText (1,QString::fromLocal8Bit(stInfo.szName));
							OSDmap.insert(QString::fromLocal8Bit("通道%1").arg(i + 1), QString::fromLocal8Bit(stInfo.szName));
						}
					}*/
				}
			}
		}
		delete[] stOut.pCameraStateInfo;
		for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
		{
			delete[] dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs;
		}
		delete[] dhOutMatrix.pstuCameras;
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
					QTreeWidgetItem* subMenu = new QTreeWidgetItem(ipcInface);
					//subMenu->setText(0, QString((const char*)ipInfo.struIPDevInfo[i].struIP.sIpV4));
					/*NET_DVR_USER_LOGIN_INFO hkLoginInfo = { 0 };
					NET_DVR_DEVICEINFO_V40 hkDeviceInfo = { 0 };
					strncpy(hkLoginInfo.sDeviceAddress, ipInfo.struIPDevInfo[i].struIP.sIpV4, 64);
					strncpy(hkLoginInfo.sUserName, (const char*)ipInfo.struIPDevInfo[i].sUserName, 64);
					strncpy(hkLoginInfo.sPassword, (const char*)ipInfo.struIPDevInfo[i].sPassword, 64);
					hkLoginInfo.wPort = ipInfo.struIPDevInfo[i].wDVRPort;
					hkLoginInfo.bUseAsynLogin = 0;
					hkLoginInfo.byJiaFaEncypt = 2;
					LONG reLog = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
					if (reLog==-1)
					{
					hkLoginInfo.byJiaFaEncypt = 0;
					reLog= NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
					if (reLog==-1)
					{
					break;
					}
					}*/
					while (ipInfo.struStreamMode[factI].uGetStream.struChanInfo.byChannel == 0)
					{
						factI++;
					}
					QString osdinfo = QString::fromLocal8Bit("通道%1").arg(factI + 1);
					subMenu->setText(0, osdinfo);
					if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_PICCFG_V40, factI + 1, &piccfg, sizeof(NET_DVR_PICCFG_V40), &reLength))
					{
						subMenu->setText(1, QString::fromLocal8Bit((const char*)piccfg.sChanName));
						//subMenu->setTextAlignment(1, 63);
						OSDmap.insert(osdinfo, QString::fromLocal8Bit((const char*)piccfg.sChanName));
					}
					//ipcInface->openPersistentEditor(subMenu, i);
					memset(&piccfg, 0, sizeof(NET_DVR_PICCFG_V40));
				}
				//if (ipInfo.struStreamMode[i].uGetStream.struChanInfo.byEnable)
				//{
				//	QTreeWidgetItem* subMenu = new QTreeWidgetItem(ipcInface);
				//	QString osdinfo = QString::fromLocal8Bit("通道%1").arg(i);
				//	subMenu->setText(0, osdinfo);
				//	if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_PICCFG_V40,, &piccfg, sizeof(NET_DVR_PICCFG_V40), &reLength))
				//	{
				//		subMenu->setText(1, QString::fromLocal8Bit((const char*)piccfg.sChanName));
				//		OSDmap.insert(i + 1, QString::fromLocal8Bit((const char*)piccfg.sChanName));
				//	}
				//	//ipcInface->openPersistentEditor(subMenu, i);
				//	memset(&piccfg, 0, sizeof(NET_DVR_PICCFG_V40));
				//}
			}
			if (ipInfo.dwDChanNum > 64)
			{
				memset(&ipInfo, 0, sizeof(NET_DVR_IPPARACFG_V40));
				if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_IPPARACFG_V40, 1, &ipInfo, sizeof(NET_DVR_IPPARACFG_V40), &reLength))
				{
					for (int i = 0, factI = 0; i < 64 && i < ipInfo.dwDChanNum; i++, factI++)
					{
						if (ipInfo.struIPDevInfo[i].byEnable)
						{
							QTreeWidgetItem* subMenu = new QTreeWidgetItem(ipcInface);
							//subMenu->setText(0, QString((const char*)ipInfo.struIPDevInfo[i].struIP.sIpV4));
							/*NET_DVR_USER_LOGIN_INFO hkLoginInfo = { 0 };
							NET_DVR_DEVICEINFO_V40 hkDeviceInfo = { 0 };
							strncpy(hkLoginInfo.sDeviceAddress, ipInfo.struIPDevInfo[i].struIP.sIpV4, 64);
							strncpy(hkLoginInfo.sUserName, (const char*)ipInfo.struIPDevInfo[i].sUserName, 64);
							strncpy(hkLoginInfo.sPassword, (const char*)ipInfo.struIPDevInfo[i].sPassword, 64);
							hkLoginInfo.wPort = ipInfo.struIPDevInfo[i].wDVRPort;
							hkLoginInfo.bUseAsynLogin = 0;
							hkLoginInfo.byJiaFaEncypt = 2;
							LONG reLog = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
							if (reLog==-1)
							{
							hkLoginInfo.byJiaFaEncypt = 0;
							reLog= NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
							if (reLog==-1)
							{
							break;
							}
							}*/
							while (ipInfo.struStreamMode[factI].uGetStream.struChanInfo.byChannel == 0)
							{
								factI++;
							}
							QString osdinfo = QString::fromLocal8Bit("通道%1").arg(factI + 1);
							subMenu->setText(0, osdinfo);
							if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_PICCFG_V40, factI + 1, &piccfg, sizeof(NET_DVR_PICCFG_V40), &reLength))
							{
								subMenu->setText(1, QString::fromLocal8Bit((const char*)piccfg.sChanName));
								OSDmap.insert(osdinfo, QString::fromLocal8Bit((const char*)piccfg.sChanName));
							}
							//ipcInface->openPersistentEditor(subMenu, i);
							memset(&piccfg, 0, sizeof(NET_DVR_PICCFG_V40));
						}
						//if (ipInfo.struStreamMode[i].uGetStream.struChanInfo.byEnable)
						//{
						//	QTreeWidgetItem* subMenu = new QTreeWidgetItem(ipcInface);
						//	QString osdinfo = QString::fromLocal8Bit("通道%1").arg(i);
						//	subMenu->setText(0, osdinfo);
						//	if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_PICCFG_V40,, &piccfg, sizeof(NET_DVR_PICCFG_V40), &reLength))
						//	{
						//		subMenu->setText(1, QString::fromLocal8Bit((const char*)piccfg.sChanName));
						//		OSDmap.insert(i + 1, QString::fromLocal8Bit((const char*)piccfg.sChanName));
						//	}
						//	//ipcInface->openPersistentEditor(subMenu, i);
						//	memset(&piccfg, 0, sizeof(NET_DVR_PICCFG_V40));
						//}
					}
				}
			}
		}
	}
	
	return 0;
}
void DeviceVideo::parentshow()
{
	parentshow_s();
}