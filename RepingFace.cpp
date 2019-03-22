#include "RepingFace.h"
#include "WindowTitle.h"
#include "HCNetSDK.h"
#include "dhnetsdk.h"
#include "dhconfigsdk.h"
#include <QTreeWidget>
#include <QToolButton>
#include <QAction>
#include <QRadioButton>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
RepingFace::RepingFace(char* ips, long loginID, char* deviceType, QWidget *parent /* = NULL */)
{
	ip = ips;
	longid = loginID;
	strcpy(DeviceType,deviceType);
	this->setWindowTitle(QString::fromLocal8Bit(ip));
	this->setWindowFlags(Qt::FramelessWindowHint);
	/*WindowTitle* wTitle = new WindowTitle(this,341 , 99);
	wTitle->setGeometry(0, 0, 341, 99);*/
	QToolButton* closeButton = new QToolButton(this);
	closeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* pActions = new QAction(closeButton);
	pActions->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/close_wnd_h.png"));
	closeButton->setIconSize(QSize(100, 50));
	closeButton->setDefaultAction(pActions);
	closeButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	closeButton->setGeometry(500 - 50, 0, 50, 30);
	connect(closeButton, &QToolButton::clicked, this, &QWidget::close);
	connect(closeButton, &QToolButton::clicked, this, &RepingFace::parentshow);
	this->resize(500, 500);
	RepWidget = new QTreeWidget(this);
	RepWidget->setColumnCount(4);
	RepWidget->setColumnWidth(1, 150);
	RepWidget->setGeometry(1, 30, 498, 468);
	QStringList Headerlist;
	Headerlist << QString::fromLocal8Bit("通道") << QString::fromLocal8Bit("录像状态(红色为不录像)")<<QString::fromLocal8Bit("手动录像")<<QString::fromLocal8Bit("定时录像");
	RepWidget->setHeaderLabels(Headerlist);
	ManualSign=false;
	PlanLogo=false;
	GetReping();
	VideoTime = new QTimer(this);
	connect(VideoTime, SIGNAL(timeout()), this, SLOT(VideoStatus()));
	VideoTime->start(1000);
	this->show();
}
void RepingFace::GetReping()
{
	RepWidget->clear();
	QTreeWidgetItem* dontkonw = new QTreeWidgetItem(RepWidget);
	dontkonw->setText(0, QString::fromLocal8Bit("一键开关"));
	QToolButton* manualButton = new QToolButton;
	manualButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* manualAction = new QAction(manualButton);
	manualAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
	manualButton->setIconSize(QSize(25, 25));
	manualButton->setDefaultAction(manualAction);
	manualButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	RepWidget->setItemWidget(dontkonw, 2, manualButton);
	connect(manualButton, SIGNAL(clicked()), this, SLOT(ManualFunc()));
	QToolButton* PlanButton = new QToolButton;
	PlanButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* PlanAction = new QAction(PlanButton);
	PlanAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
	PlanButton->setIconSize(QSize(25, 25));
	PlanButton->setDefaultAction(PlanAction);
	PlanButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	RepWidget->setItemWidget(dontkonw, 3, PlanButton);
	connect(PlanButton, SIGNAL(clicked()), this, SLOT(PlanFunc()));
	if (strncmp(DeviceType,"DH",2)==0)
	{
		char VideoState[256];
		memset(VideoState, 0, 256);
		int nRSBufferlen = 0;
		//DH_IN_MATRIX_GET_CAMERAS dhInMatrix = { 0 };
		//dhInMatrix.dwSize = sizeof(DH_IN_MATRIX_GET_CAMERAS);
		//DH_OUT_MATRIX_GET_CAMERAS dhOutMatrix = { 0 };
		//dhOutMatrix.dwSize = sizeof(DH_OUT_MATRIX_GET_CAMERAS);
		//dhOutMatrix.nMaxCameraCount = 128;
		//dhOutMatrix.pstuCameras = new DH_MATRIX_CAMERA_INFO[dhOutMatrix.nMaxCameraCount];
		//memset(dhOutMatrix.pstuCameras, 0, sizeof(DH_MATRIX_CAMERA_INFO)*(dhOutMatrix.nMaxCameraCount));
		//for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
		//{
		//	dhOutMatrix.pstuCameras[i].dwSize = sizeof(DH_MATRIX_CAMERA_INFO);
		//	dhOutMatrix.pstuCameras[i].stuRemoteDevice.dwSize = sizeof(DH_REMOTE_DEVICE);
		//	dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount = 128;
		//	dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs = new DH_VIDEO_INPUTS[dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount];
		//	memset(dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs, 0, sizeof(DH_VIDEO_INPUTS)*(dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount));
		//	dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs->dwSize = sizeof(DH_VIDEO_INPUTS);
		//}
		//if (!CLIENT_MatrixGetCameras(longid, &dhInMatrix, &dhOutMatrix, 5000))//获取前端摄像机列表信息
		//{
		//	for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
		//	{
		//		delete[] dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs;
		//	}
		//	delete[] dhOutMatrix.pstuCameras;
		//	return ;
		//}
		//int k = dhOutMatrix.nRetCameraCount;
		//while (k)
		//{
		//	DH_MATRIX_CAMERA_INFO nowDevice;
		//	nowDevice= dhOutMatrix.pstuCameras[k - 1];
		//	k--;
		//}
		//for (int i = 0; i < dhOutMatrix.nRetCameraCount; i++)
		//{
		//	if (dhOutMatrix.pstuCameras[i].stuRemoteDevice.bEnable)
		//	{
		//		DWORD statusOfnow;
		//		int reStatus;
		//		if (CLIENT_QueryRemotDevState(longid, DH_DEVSTATE_ONLINE, dhOutMatrix.pstuCameras[i].nUniqueChannel,(char*)&statusOfnow, 8, &reStatus))
		//		{
		//			if (statusOfnow)
		//			{
		//				QTreeWidgetItem* dh_subMenu = new QTreeWidgetItem(RepWidget);
		//				dh_subMenu->setText(0, QString::fromLocal8Bit("通道%1").arg(dhOutMatrix.pstuCameras[i].nUniqueChannel + 1));
		//			}
		//		}
		//	}
		//}
		/*char* group = new char[1024 * 200];
		memset(group, 0, 1024 * 200);
		CFG_VIDEOINDEVGROUP_INFO group_info;
		nRSBufferlen = sizeof(CFG_VIDEOINDEVGROUP_INFO);
		if (CLIENT_GetNewDevConfig(longid, CFG_CMD_VIDEOINDEVGROUP,0,group,1024*200,&nRSBufferlen,5000))
		{
			if (CLIENT_ParseData(CFG_CMD_VIDEOINDEVGROUP,group,&group_info,sizeof(CFG_VIDEOINDEVGROUP_INFO),NULL))
			{
				for (int i=0;i<group_info.nVideoDevNum;i++)
				{
					if (group_info.stuVideoInDevInfo[i].byStatus==1)
					{
						QTreeWidgetItem* dh_subMenu = new QTreeWidgetItem(RepWidget);
					   dh_subMenu->setText(0, QString::fromLocal8Bit("通道%1").arg(i + 1));
					}
				}
			}
		}*/
		NET_IN_GET_CAMERA_STATEINFO stIn = { 0 };
		stIn.dwSize = sizeof(NET_IN_GET_CAMERA_STATEINFO);
		stIn.bGetAllFlag = true;
		NET_OUT_GET_CAMERA_STATEINFO stOut = { 0 };
		stOut.dwSize = sizeof(NET_OUT_GET_CAMERA_STATEINFO);
		stOut.nMaxNum = 128;
		stOut.pCameraStateInfo = new NET_CAMERA_STATE_INFO[stOut.nMaxNum];
		memset(stOut.pCameraStateInfo, 0, sizeof(NET_CAMERA_STATE_INFO)*(stOut.nMaxNum));
		if (CLIENT_QueryDevInfo(longid, NET_QUERY_GET_CAMERA_STATE,&stIn,&stOut,0,2000))
		{
			for (int i=0;i<stOut.nValidNum;i++)
			{
				if (stOut.pCameraStateInfo[i].emConnectionState== EM_CAMERA_STATE_TYPE_CONNECTED)
				{
					QTreeWidgetItem* dh_subMenu = new QTreeWidgetItem(RepWidget);
					dh_subMenu->setText(0, QString::fromLocal8Bit("通道%1").arg(i + 1));
				}
			}
		}
		delete[] stOut.pCameraStateInfo;
		//nRSBufferlen = CLIENT_GetLastError() & 0x7fffffff;
		//delete[] group;
		if (CLIENT_QueryRecordState(longid, VideoState, 256, &nRSBufferlen, 2000))
		{
			QTreeWidgetItemIterator it(RepWidget, QTreeWidgetItemIterator::All);
			while (*it)
			{
				if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
				{
					int channel = (*it)->text(0).mid(2).toInt();
					QToolButton* timeButton = new QToolButton;
					timeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
					QAction* TimeAction = new QAction(timeButton);
					QToolButton* handleButtons = new QToolButton;
					handleButtons->setToolButtonStyle(Qt::ToolButtonIconOnly);
					QAction* handleActions = new QAction(handleButtons);
					QToolButton* nowLoadState = new QToolButton;
					nowLoadState->setToolButtonStyle(Qt::ToolButtonIconOnly);
					QAction* nowLoadAction = new QAction(nowLoadState);
					if (VideoState[channel - 1] == 0)
					{
						/*char* szoutBuffer = new char[1024 * 10];
						int ErrorNum;
						CFG_RECORD_INFO Recordinfo;
						if (CLIENT_GetNewDevConfig(longid, CFG_CMD_RECORD,channel,szoutBuffer,1024*10,&ErrorNum,2000))
						{
							if (CLIENT_ParseData(CFG_CMD_RECORD, szoutBuffer, &Recordinfo, sizeof(CFG_RECORD_INFO), NULL))
							{
								if
							}
						}*/
						nowLoadAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio_ing.png"));
						handleActions->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
						TimeAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
					}
					else if (VideoState[channel - 1] == 1)
					{
						nowLoadAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio.png"));
						handleActions->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
						TimeAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
					}
					else if (VideoState[channel - 1] == 2)
					{	
						nowLoadAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio.png"));
						handleActions->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
						TimeAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
					}
					nowLoadState->setIconSize(QSize(25, 25));
					nowLoadState->setDefaultAction(nowLoadAction);
					nowLoadState->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
					handleButtons->setIconSize(QSize(25, 25));
					handleButtons->setDefaultAction(handleActions);
					handleButtons->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
					connect(handleButtons, SIGNAL(clicked()), this, SLOT(startHandReping()));
					timeButton->setIconSize(QSize(25, 25));
					timeButton->setDefaultAction(TimeAction);
					timeButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
					connect(timeButton, SIGNAL(clicked()), this, SLOT(TimeingVideoFunc()));
					RepWidget->setItemWidget((*it), 1, nowLoadState);
					RepWidget->setItemWidget((*it), 2, handleButtons);
					RepWidget->setItemWidget((*it), 3, timeButton);
				}
				it++;
			}
		}
		return;
	}
	NET_DVR_IPPARACFG_V40 ipInfo;
	DWORD reLength;
	NET_DVR_CHAN_GROUP_RECORD_STATUS RecordStatus;
	DWORD ReInt;
	NET_DVR_WORKSTATE_V30 chanStatus;
	if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_IPPARACFG_V40, 0, &ipInfo, sizeof(NET_DVR_IPPARACFG_V40), &reLength))
	{
		//bool rebool = NET_DVR_GetDVRConfig(longid, NET_DVR_GET_RECORD_CHANNEL_INFO, 0, &RecordStatus, sizeof(NET_DVR_CHAN_GROUP_RECORD_STATUS), &ReInt);
		bool rebool = NET_DVR_GetDVRWorkState_V30(longid, &chanStatus);
		HandReping = new bool[ipInfo.dwDChanNum];
		int factI;
		for (int i = 0,factI=0; i < 64 && i < ipInfo.dwDChanNum; i++,factI++)
		{
			((bool*)HandReping)[i] = false;
			if (ipInfo.struIPDevInfo[i].byEnable)
			{
				QTreeWidgetItem* subMenu = new QTreeWidgetItem(RepWidget);
				while (ipInfo.struStreamMode[factI].uGetStream.struChanInfo.byChannel == 0)
				{
					factI++;
				}
				QString osdinfo = QString::fromLocal8Bit("通道%1").arg(factI + 1);
				subMenu->setText(0, osdinfo);
				if (rebool)
				{
					QToolButton* RepingButton = new QToolButton;
					RepingButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
					QAction* RepingAction = new QAction(RepingButton);
					if (chanStatus.struChanStatic[factI].byRecordStatic)
					{
						RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio.png"));
					}
					if (!chanStatus.struChanStatic[factI].byRecordStatic)
					{
						RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio_ing.png"));
					}
					RepingButton->setIconSize(QSize(25, 25));
					RepingButton->setDefaultAction(RepingAction);
					RepingButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
					RepWidget->setItemWidget(subMenu, 1, RepingButton);
				}
				QToolButton* setstart = new QToolButton;
				setstart->setToolButtonStyle(Qt::ToolButtonIconOnly);
				QAction* startAction = new QAction(setstart);
				startAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
				setstart->setIconSize(QSize(25, 25));
				setstart->setDefaultAction(startAction);
				setstart->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
				RepWidget->setItemWidget(subMenu, 2, setstart);
				connect(setstart, SIGNAL(clicked()), this, SLOT(startHandReping()));

				QToolButton* TimingVideo = new QToolButton;
				TimingVideo->setToolButtonStyle(Qt::ToolButtonIconOnly);
				QAction* TimingVideoAction = new QAction(TimingVideo);
				NET_DVR_RECORD_V40 ipcRecord;
				DWORD reapp;
				if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_RECORDCFG_V40, factI+1, &ipcRecord, sizeof(NET_DVR_RECORD_V40), &reapp))
				{
					if (ipcRecord.dwRecord)
					{
						TimingVideoAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
					}else 
					{
						TimingVideoAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
					}
				}
				TimingVideo->setIconSize(QSize(25, 25));
				TimingVideo->setDefaultAction(TimingVideoAction);
				TimingVideo->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
				RepWidget->setItemWidget(subMenu, 3, TimingVideo);
				connect(TimingVideo, SIGNAL(clicked()), this, SLOT(TimeingVideoFunc()));
			}
		}
		if (ipInfo.dwDChanNum > 64)
		{
			memset(&ipInfo, 0, sizeof(NET_DVR_IPPARACFG_V40));
			rebool = NET_DVR_GetDVRConfig(longid, NET_DVR_GET_RECORD_CHANNEL_INFO, 1, &RecordStatus, sizeof(NET_DVR_CHAN_GROUP_RECORD_STATUS), &ReInt);
			QTreeWidgetItem* subMenu = new QTreeWidgetItem(RepWidget);
			if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_IPPARACFG_V40, 1, &ipInfo, sizeof(NET_DVR_IPPARACFG_V40), &reLength))
			{
				for (int i = 0; i < 64 && i < ipInfo.dwDChanNum - 64; i++)
				{
					if (ipInfo.struIPDevInfo[i].byEnable)
					{
						subMenu->setText(0, QString((const char*)ipInfo.struIPDevInfo[i].struIP.sIpV4));
						if (rebool)
						{
							QToolButton* RepingButton = new QToolButton;
							RepingButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
							QAction* RepingAction = new QAction(RepingButton);
							if (RecordStatus.struChanStatus[i].byValid&&RecordStatus.struChanStatus[i].byRecord)
							{
								RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio.png"));
							}
							if (RecordStatus.struChanStatus[i].byValid&&RecordStatus.struChanStatus[i].byRecord == 0)
							{
								RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio_ing.png"));
							}
							RepingButton->setIconSize(QSize(25, 25));
							RepingButton->setDefaultAction(RepingAction);
							RepingButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
							RepWidget->setItemWidget(subMenu, 1, RepingButton);
						}
					}
					
				}
			}
		}
	}
	
}
void RepingFace::parentshow()
{
	VideoTime->stop();
	parentshow_s();
}
void RepingFace::startHandReping()
{
	QToolButton *button = dynamic_cast<QToolButton *>(QObject::sender()); //到信号发送者
	QModelIndex index = RepWidget->indexAt(button->pos());
	int row = index.row();
	int column = index.column();
	QTreeWidgetItemIterator it(RepWidget, QTreeWidgetItemIterator::All);
	it += row;
	QString ipstring = (*it)->text(0);
	QAction* NowAction;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		DH_IN_MATRIX_GET_CAMERAS dhInMatrix = { 0 };
		dhInMatrix.dwSize = sizeof(DH_IN_MATRIX_GET_CAMERAS);
		DH_OUT_MATRIX_GET_CAMERAS dhOutMatrix = { 0 };
		dhOutMatrix.dwSize = sizeof(DH_OUT_MATRIX_GET_CAMERAS);
		dhOutMatrix.nMaxCameraCount = 128;
		dhOutMatrix.pstuCameras = new DH_MATRIX_CAMERA_INFO[dhOutMatrix.nMaxCameraCount];
		memset(dhOutMatrix.pstuCameras, 0, sizeof(DH_MATRIX_CAMERA_INFO)*(dhOutMatrix.nMaxCameraCount));
		for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
		{
			dhOutMatrix.pstuCameras[i].dwSize = sizeof(DH_MATRIX_CAMERA_INFO);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.dwSize = sizeof(DH_REMOTE_DEVICE);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount = 128;
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs = new DH_VIDEO_INPUTS[dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount];
			memset(dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs, 0, sizeof(DH_VIDEO_INPUTS)*(dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount));
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs->dwSize = sizeof(DH_VIDEO_INPUTS);
		}
		if (CLIENT_MatrixGetCameras(longid, &dhInMatrix, &dhOutMatrix, 5000))
		{
		}
		int nChannel = dhOutMatrix.nRetCameraCount;
		char* VideoState = new char[nChannel];
		memset(VideoState, 0, nChannel);
		int nRSBufferlen = 0;
		if (CLIENT_QueryRecordState(longid, VideoState, nChannel, &nRSBufferlen, 2000))
		{
			int dwChannel = ipstring.mid(2).toLong();
			if (VideoState[dwChannel-1]==1)
			{
				VideoState[dwChannel - 1] = 2;
			}else if (VideoState[dwChannel-1]!=1)
			{
				VideoState[dwChannel - 1] = 1;
			}
			if (CLIENT_SetupRecordState(longid,VideoState,nChannel))
			{
				NowAction = button->defaultAction();
				if (CLIENT_QueryRecordState(longid, VideoState, nChannel, &nRSBufferlen, 2000))
				{
					if (VideoState[dwChannel - 1] == 1)
					{
						NowAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
					}
					else {
						NowAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
					}
				}
				button->setIconSize(QSize(25, 25));
				button->setDefaultAction(NowAction);
				button->update();
			}
		}
		delete[] VideoState;
		return;
	}
	else {
		if (((bool*)HandReping)[row])
		{
			if (NET_DVR_StopDVRRecord(longid, ipstring.mid(2).toLong()))
			{
				NowAction = button->defaultAction();
				NowAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
				button->setIconSize(QSize(25, 25));
				button->setDefaultAction(NowAction);
				button->update();
				((bool*)HandReping)[row] = false;
				//GetReping_id(row);
			}
			return;
		}
		if (!((bool*)HandReping)[row])
		{
			if (NET_DVR_StartDVRRecord(longid, ipstring.mid(2).toLong(), 0))
			{
				NowAction = button->defaultAction();
				NowAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
				button->setIconSize(QSize(25, 25));
				button->setDefaultAction(NowAction);
				button->update();
				((bool*)HandReping)[row] = true;
				//GetReping_id(row);
			}
			return;
		}
	}
}
void RepingFace::TimeingVideoFunc()
{
	QToolButton *button = dynamic_cast<QToolButton *>(QObject::sender()); //到信号发送者
	QModelIndex index = RepWidget->indexAt(button->pos());
	int row = index.row();
	int column = index.column();
	QTreeWidgetItemIterator it(RepWidget, QTreeWidgetItemIterator::All);
	it += row;
	QString ipstring = (*it)->text(0);
	QAction* NowAction;
	NET_DVR_RECORD_V40 ipcRecord;
	DWORD reapp;
	if (strncmp(DeviceType, "DH", 2)==0)
	{
		DH_IN_MATRIX_GET_CAMERAS dhInMatrix = { 0 };
		dhInMatrix.dwSize = sizeof(DH_IN_MATRIX_GET_CAMERAS);
		DH_OUT_MATRIX_GET_CAMERAS dhOutMatrix = { 0 };
		dhOutMatrix.dwSize = sizeof(DH_OUT_MATRIX_GET_CAMERAS);
		dhOutMatrix.nMaxCameraCount = 128;
		dhOutMatrix.pstuCameras = new DH_MATRIX_CAMERA_INFO[dhOutMatrix.nMaxCameraCount];
		memset(dhOutMatrix.pstuCameras, 0, sizeof(DH_MATRIX_CAMERA_INFO)*(dhOutMatrix.nMaxCameraCount));
		for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
		{
			dhOutMatrix.pstuCameras[i].dwSize = sizeof(DH_MATRIX_CAMERA_INFO);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.dwSize = sizeof(DH_REMOTE_DEVICE);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount = 128;
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs = new DH_VIDEO_INPUTS[dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount];
			memset(dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs, 0, sizeof(DH_VIDEO_INPUTS)*(dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount));
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs->dwSize = sizeof(DH_VIDEO_INPUTS);
		}
		if (CLIENT_MatrixGetCameras(longid, &dhInMatrix, &dhOutMatrix, 5000))
		{
		}
		int nChannel = dhOutMatrix.nRetCameraCount;
		char* VideoState = new char[nChannel];
		memset(VideoState, 0, nChannel);
		int nRSBufferlen = 0;
		if (CLIENT_QueryRecordState(longid, VideoState, nChannel, &nRSBufferlen, 2000))
		{
			int dwChannel = ipstring.mid(2).toLong();
			if (VideoState[dwChannel - 1] == 2)
			{
				VideoState[dwChannel - 1] = 0;
			}
			else if (VideoState[dwChannel - 1] != 2)
			{
				VideoState[dwChannel - 1] = 2;
			}
			if (CLIENT_SetupRecordState(longid, VideoState, nChannel))
			{
				NowAction = button->defaultAction();
				if (VideoState[dwChannel - 1] == 2)
				{
					NowAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
				}
				else {
					NowAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
				}
				button->setIconSize(QSize(25, 25));
				button->setDefaultAction(NowAction);
				button->update();
			}
		}
		delete[] VideoState;
		return;
	}
	else {
		if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_RECORDCFG_V40, ipstring.mid(2).toLong(), &ipcRecord, sizeof(NET_DVR_RECORD_V40), &reapp))
		{
			if (ipcRecord.dwRecord)
			{
				ipcRecord.dwRecord = 0;
				if (NET_DVR_SetDVRConfig(longid, NET_DVR_SET_RECORDCFG_V40, ipstring.mid(2).toLong(), &ipcRecord, sizeof(NET_DVR_RECORD_V40)))
				{
					NowAction = button->defaultAction();
					NowAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
					button->setIconSize(QSize(25, 25));
					button->setDefaultAction(NowAction);
					button->update();
					//GetReping_id(row);
				}
				return;
			}
			if (!ipcRecord.dwRecord)
			{
				ipcRecord.dwRecord = 1;
				if (NET_DVR_SetDVRConfig(longid, NET_DVR_SET_RECORDCFG_V40, ipstring.mid(2).toLong(), &ipcRecord, sizeof(NET_DVR_RECORD_V40)))
				{
					NowAction = button->defaultAction();
					NowAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
					button->setIconSize(QSize(25, 25));
					button->setDefaultAction(NowAction);
					button->update();
					//GetReping_id(row);
				}
				return;
			}
		}
	}
}
void RepingFace::VideoStatus()
{
	QTreeWidgetItemIterator it(RepWidget, QTreeWidgetItemIterator::All);
	NET_DVR_WORKSTATE_V30 chanStatus;
	int dwChannel;
	bool rebool;
	char VideoState[256];
	memset(VideoState, 0, 256);
	int nRSBufferlen = 0;
	if (strncmp(DeviceType, "DH", 2) == 0)
	{
		rebool = CLIENT_QueryRecordState(longid, VideoState, 256, &nRSBufferlen, 2000);
	}else
	{
		rebool = NET_DVR_GetDVRWorkState_V30(longid, &chanStatus);
	}
	while (*it)
	{
		memset(&chanStatus, 0, sizeof(NET_DVR_WORKSTATE_V30));
		dwChannel = (*it)->text(0).mid(2).toLong();
		//rebool = NET_DVR_GetDVRWorkState_V30(longid, &chanStatus);
		if (rebool&&!(*it)->text(0).isEmpty()&&(*it)->text(0)!=QString::fromLocal8Bit("一键开关"))
		{
			QToolButton* RepingButton = new QToolButton;
			RepingButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
			QAction* RepingAction = new QAction(RepingButton);
			if (strncmp(DeviceType, "DH", 2) == 0)
			{
				if (VideoState[dwChannel-1]==0)
				{
					RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio_ing.png"));
				}else if (VideoState[dwChannel-1]!=0)
				{
					RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio.png"));
				}
			}
			else {
				if (chanStatus.struChanStatic[dwChannel - 1].byRecordStatic)
				{
					RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio.png"));
				}
				if (!chanStatus.struChanStatic[dwChannel - 1].byRecordStatic)
				{
					RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio_ing.png"));
				}
			}
			RepingButton->setIconSize(QSize(25, 25));
			RepingButton->setDefaultAction(RepingAction);
			RepingButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
			RepWidget->setItemWidget(*it, 1, RepingButton);
		}
		it++;
	}
}
void RepingFace::ManualFunc()
{
	QToolButton* paButton = qobject_cast<QToolButton*>(sender());
	QAction* paAction = paButton->defaultAction();
	QToolButton* childButton;
	QAction*childAction;
	int hkError;
	QTreeWidgetItemIterator it(RepWidget, QTreeWidgetItemIterator::All);
	if (strncmp(DeviceType,"DH",2)==0)
	{
		DH_IN_MATRIX_GET_CAMERAS dhInMatrix = { 0 };
		dhInMatrix.dwSize = sizeof(DH_IN_MATRIX_GET_CAMERAS);
		DH_OUT_MATRIX_GET_CAMERAS dhOutMatrix = { 0 };
		dhOutMatrix.dwSize = sizeof(DH_OUT_MATRIX_GET_CAMERAS);
		dhOutMatrix.nMaxCameraCount = 128;
		dhOutMatrix.pstuCameras = new DH_MATRIX_CAMERA_INFO[dhOutMatrix.nMaxCameraCount];
		memset(dhOutMatrix.pstuCameras, 0, sizeof(DH_MATRIX_CAMERA_INFO)*(dhOutMatrix.nMaxCameraCount));
		for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
		{
			dhOutMatrix.pstuCameras[i].dwSize = sizeof(DH_MATRIX_CAMERA_INFO);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.dwSize = sizeof(DH_REMOTE_DEVICE);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount = 128;
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs = new DH_VIDEO_INPUTS[dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount];
			memset(dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs, 0, sizeof(DH_VIDEO_INPUTS)*(dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount));
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs->dwSize = sizeof(DH_VIDEO_INPUTS);
		}
		if (CLIENT_MatrixGetCameras(longid, &dhInMatrix, &dhOutMatrix, 5000))
		{
		}
		int nChannel = dhOutMatrix.nRetCameraCount;
		char* VideoState=new char[nChannel];
		memset(VideoState, 0, nChannel);
		int nRSBufferlen = 0;
		if (ManualSign)
		{
			memset(VideoState, 2, nChannel);
			if (CLIENT_SetupRecordState(longid,VideoState, nChannel))
			{
				paButton->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
				paButton->update();
				ManualSign = false;
				while (*it)
				{
					if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
					{
						childButton = (QToolButton*)RepWidget->itemWidget(*it, 2);
						childAction = childButton->defaultAction();
						childAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
						childButton->update();
					}
					it++;
				}
			}
		}else if (!ManualSign)
		{
			memset(VideoState, 1, nChannel);
			if (CLIENT_SetupRecordState(longid,VideoState, nChannel))
			{
				ManualSign = true;
				paButton->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
				paButton->update();
				while (*it)
				{
					if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
					{
						childButton = (QToolButton*)RepWidget->itemWidget(*it, 2);
						childAction = childButton->defaultAction();
						childAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
						childButton->update();
					}
					it++;
				}
			}
		}
		delete[] VideoState;
		return;
	} 
	else
	{
		if (ManualSign)
		{
			if (NET_DVR_StopDVRRecord(longid, 0xff00))
			{
				paButton->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
				paButton->update();
				ManualSign = false;
				while (*it)
				{
					if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
					{
						childButton = (QToolButton*)RepWidget->itemWidget(*it, 2);
						childAction = childButton->defaultAction();
						childAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
						childButton->update();
					}
					it++;
				}
			}
			else
			{
				while (*it)
				{
					if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
					{
						if (NET_DVR_StopDVRRecord(longid, (*it)->text(0).mid(2).toInt()))
						{
							paButton->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
							paButton->update();
							ManualSign = false;
							childButton = (QToolButton*)RepWidget->itemWidget(*it, 2);
							childAction = childButton->defaultAction();
							childAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
							childButton->update();
						}
					}
					it++;
				}
			}
			return;
		}
		else if (!ManualSign)
		{
			if (NET_DVR_StartDVRRecord(longid, 0xff00, 0))
			{
				paButton->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
				paButton->update();
				ManualSign = true;
				while (*it)
				{
					if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
					{
						childButton = (QToolButton*)RepWidget->itemWidget(*it, 2);
						childAction = childButton->defaultAction();
						childAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
						childButton->update();
					}
					it++;
				}
			}
			else
			{
				while (*it)
				{
					if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
					{
						if (NET_DVR_StartDVRRecord(longid, (*it)->text(0).mid(2).toInt(), 0))
						{
							paButton->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
							paButton->update();
							ManualSign = true;
							childButton = (QToolButton*)RepWidget->itemWidget(*it, 2);
							childAction = childButton->defaultAction();
							childAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
							childButton->update();
						}
					}
					it++;
				}
			}
			hkError = NET_DVR_GetLastError();
			return;
		}
	}
}
void RepingFace::PlanFunc()
{
	QTreeWidgetItemIterator it(RepWidget, QTreeWidgetItemIterator::All);
	int dwChannel;
	QToolButton* paButton = qobject_cast<QToolButton*>(sender());
	QAction* paAction = paButton->defaultAction();
	QToolButton* childButton;
	QAction*childAction;
	if (strncmp(DeviceType,"DH",2)==0)
	{
		DH_IN_MATRIX_GET_CAMERAS dhInMatrix = { 0 };
		dhInMatrix.dwSize = sizeof(DH_IN_MATRIX_GET_CAMERAS);
		DH_OUT_MATRIX_GET_CAMERAS dhOutMatrix = { 0 };
		dhOutMatrix.dwSize = sizeof(DH_OUT_MATRIX_GET_CAMERAS);
		dhOutMatrix.nMaxCameraCount = 128;
		dhOutMatrix.pstuCameras = new DH_MATRIX_CAMERA_INFO[dhOutMatrix.nMaxCameraCount];
		memset(dhOutMatrix.pstuCameras, 0, sizeof(DH_MATRIX_CAMERA_INFO)*(dhOutMatrix.nMaxCameraCount));
		for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
		{
			dhOutMatrix.pstuCameras[i].dwSize = sizeof(DH_MATRIX_CAMERA_INFO);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.dwSize = sizeof(DH_REMOTE_DEVICE);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount = 128;
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs = new DH_VIDEO_INPUTS[dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount];
			memset(dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs, 0, sizeof(DH_VIDEO_INPUTS)*(dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount));
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs->dwSize = sizeof(DH_VIDEO_INPUTS);
		}
		if (CLIENT_MatrixGetCameras(longid, &dhInMatrix, &dhOutMatrix, 5000))
		{
		}
		int nChannel = dhOutMatrix.nRetCameraCount;
		char* VideoState = new char[nChannel];
		memset(VideoState, 0, nChannel);
		int nRSBufferlen = 0;
		if (PlanLogo)
		{
			memset(VideoState, 0, nChannel);
			if (CLIENT_SetupRecordState(longid, VideoState, nChannel))
			{
				while (*it)
				{
					if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
					{
						childButton = (QToolButton*)RepWidget->itemWidget(*it, 3);
						childAction = childButton->defaultAction();
						childAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
						childButton->update();
					}
					it++;
				}
				paAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
				paButton->update();
				PlanLogo = false;
			}
			delete[] VideoState;
			return;
		}
		else if (!PlanLogo)
		{
			memset(VideoState, 2, nChannel);
			if (CLIENT_SetupRecordState(longid, VideoState, nChannel))
			{
				while (*it)
				{
					if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
					{
						childButton = (QToolButton*)RepWidget->itemWidget(*it, 3);
						childAction = childButton->defaultAction();
						childAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
						childButton->update();
					}
					it++;
				}
				paAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
				paButton->update();
				PlanLogo = true;
			}
			delete[] VideoState;
			return;
		}
	} 
	else
	{
		if (PlanLogo)
		{
			while (*it)
			{
				if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
				{
					dwChannel = (*it)->text(0).mid(2).toInt();
					NET_DVR_RECORD_V40 ipcRecord;
					DWORD reapp;
					if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_RECORDCFG_V40, dwChannel, &ipcRecord, sizeof(NET_DVR_RECORD_V40), &reapp))
					{
						ipcRecord.dwRecord = 0;
						if (NET_DVR_SetDVRConfig(longid, NET_DVR_SET_RECORDCFG_V40, dwChannel, &ipcRecord, sizeof(NET_DVR_RECORD_V40)))
						{
							childButton = (QToolButton*)RepWidget->itemWidget(*it, 3);
							childAction = childButton->defaultAction();
							childAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
							childButton->update();
						}
					}
				}
				it++;
			}
			paAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
			paButton->update();
			PlanLogo = false;
			return;
		}
		else if (!PlanLogo)
		{
			while (*it)
			{
				if (!(*it)->text(0).isEmpty() && (*it)->text(0) != QString::fromLocal8Bit("一键开关"))
				{
					dwChannel = (*it)->text(0).mid(2).toInt();
					NET_DVR_RECORD_V40 ipcRecord;
					DWORD reapp;
					if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_RECORDCFG_V40, dwChannel, &ipcRecord, sizeof(NET_DVR_RECORD_V40), &reapp))
					{
						ipcRecord.dwRecord = 1;
						if (NET_DVR_SetDVRConfig(longid, NET_DVR_SET_RECORDCFG_V40, dwChannel, &ipcRecord, sizeof(NET_DVR_RECORD_V40)))
						{
							childButton = (QToolButton*)RepWidget->itemWidget(*it, 3);
							childAction = childButton->defaultAction();
							childAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
							childButton->update();
						}
					}
				}
				it++;
			}
			paAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
			paButton->update();
			PlanLogo = true;
			return;
		}
	}
}
void RepingFace::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastmousePosition = event->globalPos();
	}
}
void RepingFace::mouseMoveEvent(QMouseEvent* event)
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
void RepingFace::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
	}
}
void RepingFace::GetReping_id(int a)
{
	QTreeWidgetItemIterator it(RepWidget, QTreeWidgetItemIterator::All);
	it += a;
	int dwChannel = (*it)->text(0).mid(2).toLong();
	NET_DVR_IPPARACFG_V40 ipInfo;
	DWORD reLength;
	NET_DVR_CHAN_GROUP_RECORD_STATUS RecordStatus;
	DWORD ReInt;
	NET_DVR_WORKSTATE_V30 chanStatus;
    bool rebool = NET_DVR_GetDVRWorkState_V30(longid, &chanStatus);
	if (rebool)
	{
		QToolButton* RepingButton = new QToolButton;
		RepingButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		QAction* RepingAction = new QAction(RepingButton);
		if (chanStatus.struChanStatic[dwChannel-1].byRecordStatic)
		{
			RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio.png"));
		}
		if (!chanStatus.struChanStatic[dwChannel-1].byRecordStatic)
		{
			RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio_ing.png"));
		}
		RepingButton->setIconSize(QSize(25, 25));
		RepingButton->setDefaultAction(RepingAction);
		RepingButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
		RepWidget->setItemWidget(*it, 1, RepingButton);
	}
	NET_DVR_RECORD_V40 ipcRecord;
	DWORD reapp;
	if (NET_DVR_GetDVRConfig(longid, NET_DVR_GET_RECORDCFG_V40, dwChannel, &ipcRecord, sizeof(NET_DVR_RECORD_V40), &reapp))
	{
		QToolButton* DontknowButton = new QToolButton;
		DontknowButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		QAction* DontknowAction = new QAction(DontknowButton);
		if (ipcRecord.dwRecord)
		{
			DontknowAction->setIcon(QIcon(":/MemoryManager/Resources/startReping.png"));
		}
		if (!ipcRecord.dwRecord)
		{
			DontknowAction->setIcon(QIcon(":/MemoryManager/Resources/stopReping.png"));
		}
		DontknowButton->setIconSize(QSize(25, 25));
		DontknowButton->setDefaultAction(DontknowAction);
		DontknowButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
		RepWidget->setItemWidget(*it, 3, DontknowButton);
		connect(DontknowButton, SIGNAL(clicked()), this, SLOT(TimeingVideoFunc()));
	}

}
void RepingFace::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.drawRect(QRect(1, 1, 500 - 2, 500 - 2));
}
