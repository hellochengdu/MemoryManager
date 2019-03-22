#include "MemoryManager.h"
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTreeWidget>
#include <QToolButton>
#include <QMovie>
#include <QMouseEvent>
#include <QPainter>
#include <QMap>
#include <QThread>
#include <QApplication>
#include <QDesktopWidget>
#include <QSqlError>
#include<QtPlugin>
#include <QDebug>
#include "login.h"
#include "HCNetSDK.h"
#include "dhnetsdk.h"
#include "CustomQlabel.h"
#include "deviceVideo.h"
#include "managementInterface.h"
#include "MemroyPlay.h"
#include "WindowTitle.h"
#include "RepingFace.h"
#define WIDTHS 720
#define HIGTHS 800
//Q_IMPORT_PLUGIN(qsqlite)
MemoryManager::MemoryManager(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	/*WindowTitle* wTitle = new WindowTitle(this, WIDTHS, 99);
	wTitle->setGeometry(0, 0, WIDTHS, 99);
	wTitle->show();*/
	setWindowTitle(QString::fromLocal8Bit("存储管理工具"));
	setWindowIcon(QIcon(":/MemoryManager/Resources/JF-NAS.ico"));
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect screenRect = desktopWidget->screenGeometry();
	int widthWidget = screenRect.width();
	int heightWidget = screenRect.height();
	x = widthWidget/ 2 - 300;
	y = heightWidget / 2 - 400;
	this->move(x, y);
	//this->setStyleSheet("background-image:url(:/MemoryManager/Resources/wnd_button_bk/player/default_bk.png);");
	//设置背景图片
	this->setAutoFillBackground(false); // 这句要加上, 否则可能显示不出背景图.
	/*QPalette palette = this->palette();
	palette.setBrush(QPalette::Window,QBrush(QPixmap(":/MemoryManager/Resources/wnd_button_bk/player/default_bk1.png").scaled(// 缩放背景图.
			this->size(),
			Qt::IgnoreAspectRatio,
			Qt::SmoothTransformation)));             // 使用平滑的缩放方式
	this->setPalette(palette);*/                           // 给widget加上背景图
	//按钮显示图标
    addButton = new QToolButton(this);
	pushButton = new QToolButton(this);
	refreshButton = new QToolButton(this);
	addButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	pushButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	refreshButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* pAction = new QAction(addButton);
	pAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/add.png"));
	addButton->setIconSize(QSize(100, 50));
	addButton->setDefaultAction(pAction);
	addButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	QAction* aAction = new QAction(pushButton);
	aAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/delete.png"));
	pushButton->setIconSize(QSize(100, 50));
	pushButton->setDefaultAction(aAction);
	pushButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	QAction* refreshAction = new QAction(refreshButton);
	refreshAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/re_get_device.png"));
	refreshButton->setIconSize(QSize(100, 50));
	refreshButton->setDefaultAction(refreshAction);
	refreshButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");

	QToolButton* closeButton = new QToolButton(this);
	closeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* pActions = new QAction(closeButton);
	pActions->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/close_wnd_h.png"));
	closeButton->setIconSize(QSize(100, 50));
	closeButton->setDefaultAction(pActions);
	closeButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	closeButton->setGeometry(WIDTHS - 50, 0, 50, 30);
	closeButton->show();
	connect(closeButton, SIGNAL(clicked()), this, SLOT(beforeClose()));

	//设备列表
	deviceInfo = new QTreeWidget(this);
	//deviceInfo->setStyleSheet("background:transparent;");
	deviceInfo->setColumnCount(7);
	addButton->setGeometry(570, 30, 70, 30);
	addButton->show();
	pushButton->setGeometry(640, 30, 70, 30);
	pushButton->show();
	refreshButton->setGeometry(500, 30, 70, 30);
	deviceInfo->setGeometry(10, 60, 700, 740);
	QStringList sclist;
	sclist << QString::fromLocal8Bit("存储设备")<< QString::fromLocal8Bit("在线状态")<<QString::fromLocal8Bit("录像情况")<<QString::fromLocal8Bit("磁盘空间大小")<<QString::fromLocal8Bit("通道标签")<<QString::fromLocal8Bit("录像设置") << QString::fromLocal8Bit("录像回放");
	deviceInfo->setHeaderLabels(sclist);
	this->resize(WIDTHS, HIGTHS);

	connect(deviceInfo, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(managerInterface(QTreeWidgetItem*, int)));
	connect(deviceInfo, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(doubleFunc(QTreeWidgetItem*,int)));
	connect(addButton, SIGNAL(clicked()), this, SLOT(construction()));
	connect(pushButton, SIGNAL(clicked()), this, SLOT(pushFction()));
	connect(refreshButton, SIGNAL(clicked()), this, SLOT(showSelectedImg()));
	//sdk 初始化
	CLIENT_InitEx(NULL, 0, NULL);
	NET_PARAM netParam = { 0 };
	netParam.nConnectTime = 2000;
	netParam.nConnectTryNum = 1;
	CLIENT_SetNetworkParam(&netParam);
	NET_DVR_Init();        //初始化
	NET_DVR_SetReconnect(2000, 0);
	//连接数据库 读取数据
	ConnectDatabase();
	ReadDatabase();
	StatusofIP();
}
long  MemoryManager::loginFunction(char* ip, char* nport, char* adm, char*psd,char* Dev)
{
	int port = atoi(nport);
	LONG qlogin=0;
	NET_DEVICEINFO_Ex DHDeviceInfo = {0};
	NET_DVR_USER_LOGIN_INFO hkLoginInfo = { 0 };
	NET_DVR_DEVICEINFO_V40 hkDeviceInfo = { 0 };
	strncpy(hkLoginInfo.sDeviceAddress, ip, 64);
	strncpy(hkLoginInfo.sUserName, adm, 64);
	strncpy(hkLoginInfo.sPassword, psd, 64);
	hkLoginInfo.wPort = port;
	hkLoginInfo.bUseAsynLogin = 0;
	int nerror;
	char devstyle[10];
	if (Dev!=NULL)
	{
		if (strcmp(Dev, "DH28") == 0)
		{
			int nSpecialValue = 90106;
			qlogin = CLIENT_LoginEx2(ip, port, adm, psd, (EM_LOGIN_SPAC_CAP_TYPE)28, &nSpecialValue, &DHDeviceInfo, &nerror);
			if (!qlogin)
			{
				//QMessageBox::warning(NULL, QString::fromLocal8Bit("登录提示"), QString::fromLocal8Bit("失败:不符合K系、V系登录条件"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return -1;
			}
		}else if (strcmp(Dev,"DH0")==0)
		{
			qlogin = CLIENT_LoginEx2(ip, port, adm, psd, (EM_LOGIN_SPAC_CAP_TYPE)0, NULL, &DHDeviceInfo, &nerror);
			if (!qlogin)
			{
				//QMessageBox::warning(NULL, QString::fromLocal8Bit("登录提示"), QString::fromLocal8Bit("失败:不符合K系、V系登录条件"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return -1;
			}
		}
		else if (strcmp(Dev,"HK2")==0)
		{
			hkLoginInfo.byJiaFaEncypt = 2;
			qlogin = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
			if (qlogin==-1)
			{
				//QMessageBox::warning(NULL, QString::fromLocal8Bit("登录提示"), QString::fromLocal8Bit("失败:不符合K系、V系登录条件"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return -1;
			}
		}else if (strcmp(Dev,"HK1")==0)
		{
			hkLoginInfo.byJiaFaEncypt = 1;
			qlogin = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
			if (qlogin == -1)
			{
				//QMessageBox::warning(NULL, QString::fromLocal8Bit("登录提示"), QString::fromLocal8Bit("失败:不符合K系、V系登录条件"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return -1;
			}
		}else if (strcmp(Dev,"HK0")==0)
		{
			hkLoginInfo.byJiaFaEncypt = 0;
			qlogin = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
			if (qlogin == -1)
			{
				//QMessageBox::warning(NULL, QString::fromLocal8Bit("登录提示"), QString::fromLocal8Bit("失败:不符合K系、V系登录条件"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return -1;
			}
		}else
		{
			int nSpecialValue = 90106;
			ZeroMemory(devstyle, 10);
			qlogin = CLIENT_LoginEx2(ip, port, adm, psd, (EM_LOGIN_SPAC_CAP_TYPE)28, &nSpecialValue, &DHDeviceInfo, &nerror);
			if (!qlogin)
			{
				qlogin = CLIENT_LoginEx2(ip, port, adm, psd, (EM_LOGIN_SPAC_CAP_TYPE)0, NULL, &DHDeviceInfo, &nerror);
				if (!qlogin)
				{
					hkLoginInfo.byJiaFaEncypt = 2;
					//qlogin = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
					qlogin = -1;
					nerror = NET_DVR_GetLastError();
					if (qlogin == -1)
					{
						hkLoginInfo.byJiaFaEncypt = 1;
						qlogin = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
						if (qlogin == -1)
						{
							//hkLoginInfo.byJiaFaEncypt = 0;
							//qlogin = NET_DVR_Login_V40(&hkLoginInfo, &hkDeviceInfo);
							/*if (qlogin != -1)
							{
								strncpy(Dev, "HK0", 3);
							}
							else
							{*/
								nerror = CLIENT_GetLastError()&(0x7fffffff);
								nerror = NET_DVR_GetLastError();
								//QMessageBox::warning(NULL, QString::fromLocal8Bit("登录提示"), QString::fromLocal8Bit("失败:不符合K系、V系登录条件"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
								return -1;
							//}
						}
						else
						{
							strncpy(Dev, "HK1", 3);
						}
					}
					else
					{
						strncpy(Dev, "HK2", 3);
					}
				}
				else
				{
					strncpy(devstyle, "DH0", 3);
				}
			}
			else
			{
				strncpy(Dev, "DH28", 4);
			}
		}

	}
	//WriteDatabase(ip, qlogin, port, adm, psd, devstyle);
	//ReadDatabase(ip);
	return qlogin;
}
void MemoryManager::ConnectDatabase()
{
	char work_path_db[_MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, work_path_db, _MAX_PATH);  //得到程序模块.exe全路径  
	QString sExeFilePath_db = QString::fromLocal8Bit(work_path_db);
	sExeFilePath_db.remove("MemoryManager.exe");
	sExeFilePath_db.append("data\\vrc.db");
	QByteArray byteArr_db = sExeFilePath_db.toLocal8Bit();
	db = nullptr;
	if (!QFile::exists(byteArr_db.data()))
	{
		HANDLE dbHandle = CreateFileA(byteArr_db.data(), GENERIC_ALL, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, NULL, NULL);
		if (dbHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(dbHandle);
		}else if (dbHandle == INVALID_HANDLE_VALUE)
		{
			QMessageBox::warning(NULL, QString::fromLocal8Bit("数据库提示"), QString::fromLocal8Bit("生成数据库失败"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	if (!db)
	{
		db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
		qDebug() << "drivers" << QSqlDatabase::drivers();
		db->setDatabaseName(byteArr_db.data());//固定位置
		QSqlError sqle;
		if (!db->open())
		{
			sqle=db->lastError();
			if (sqle.isValid())//发生错误时isValid()返回true  
			{
				QString text = sqle.text();
				QMessageBox::warning(NULL, "提示", text, QMessageBox::Ok);
			}

			QMessageBox::warning(NULL, QString::fromLocal8Bit("数据库提示"), QString::fromLocal8Bit("打开数据库失败"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	db_query = new QSqlQuery(*db);
	if (!db_query->exec("create table Device_Sheet(ip char(15) not null primary key,nport int not null,admin_h varchar(255) not null,pw_h varchar(255) not null,devType varchar(255) not null,loginID int)"))
	{
		//QMessageBox::warning(NULL, QString::fromLocal8Bit("数据库提示"), QString::fromLocal8Bit("生成表格失败"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}
void MemoryManager::WriteDatabase(char* ip, LONG LoginID, int nport, char* adm, char* psd, char*Dev)
{
	char deviceinfo[200];
	ZeroMemory(deviceinfo, 200);
	if (!nport)
	{
		ZeroMemory(deviceinfo, 200);
		sprintf(deviceinfo, "update Device_Sheet set loginID='%d' where ip='%s'", LoginID, ip);
		if (!db_query->exec(deviceinfo))
		{
			QMessageBox::warning(NULL, QString::fromLocal8Bit("数据库提示"), QString::fromLocal8Bit("更新数据失败"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}else {
		ZeroMemory(deviceinfo, 200);
		sprintf(deviceinfo, "insert into Device_Sheet(ip,nport,admin_h,pw_h,devType,loginID) values('%s','%d','%s','%s','%s','%d')", ip, nport, adm, psd, Dev, LoginID);
		if (!db_query->exec(deviceinfo))
		{
			QMessageBox::warning(NULL, QString::fromLocal8Bit("数据库提示"), QString::fromLocal8Bit("写入数据失败"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	return;
}
void MemoryManager::construction()
{
	Login* newLogin= new Login(this);
	newLogin->move(x+200, y+300);
	connect(newLogin, SIGNAL(CancelSignal()), this, SLOT(CancelSlot()));
	connect(newLogin, SIGNAL(SureSignal(QString,QString,QString,QString)), this, SLOT(SureSlot(QString,QString,QString,QString)));
	connect(newLogin, SIGNAL(parentshow_s()), this, SLOT(reshow()));
	connect(this, SIGNAL(loginsuccess()), newLogin, SLOT(succed()));
}
void MemoryManager::pushFction()
{
	QList<QTreeWidgetItem*>items = deviceInfo->selectedItems();
	int count = items.count();
	for (int i = 0; i < count; i++)
	{
		QTreeWidgetItem* SelectedItem = items[i];
		QString ipString = SelectedItem->text(0);
		deviceInfo->takeTopLevelItem(deviceInfo->indexOfTopLevelItem(SelectedItem));
		deleteData(ipString.toLatin1().data());
	}
}
void MemoryManager::showSelectedImg()
{
	refreshButton->setEnabled(false);
	//deviceInfo->setStyleSheet("background-image:url(:/MemoryManager/Resources/wnd_button_bk/player/default_bk111.png);");
	StatusofIP();
}
void MemoryManager::managerInterface(QTreeWidgetItem*qtwitem, int num)
{
	if (num!=4)
	{
		return;
	}
}
void MemoryManager::doubleFunc(QTreeWidgetItem*qtwitem, int num)
{
	if (num!=0)
	{
		return;
	}
}
void MemoryManager::CancelSlot()
{
	
}
void MemoryManager::SureSlot(QString ips,QString nports,QString admins,QString pas)
{
	char ipc[64];
	strcpy(ipc,ips.toLatin1().data());
	char npc[64];
	strcpy(npc,nports.toLatin1().data());
	char apc[64];
	strcpy(apc,admins.toLatin1().data());
	char ppc[64];
	strcpy(ppc, pas.toLatin1().data());
	char nDev[64];
	memset(nDev, 0, 64);
	LONG logID=loginFunction(ipc, npc, apc, ppc, nDev);
	loginsuccess();
	if (logID!=-1)
	{
		WriteDatabase(ipc, logID,atoi(npc),apc,ppc, nDev);
		//QMessageBox::about(NULL, QString::fromLocal8Bit("设备添加"), QString::fromLocal8Bit("添加成功"));
		StatusofIP();
		return;
	}
	QMessageBox::about(NULL, QString::fromLocal8Bit("设备添加"), QString::fromLocal8Bit("登录失败"));
}
void MemoryManager::ReadDatabase(char* ip)
{
	char deviceinfo[200];
	ZeroMemory(deviceinfo, 200);
	if (ip==NULL)
	{
		sprintf(deviceinfo, "select * from Device_Sheet");
	}
	else {
		sprintf(deviceinfo, "select * from Device_Sheet where ip='%s'", ip);
	}
	db_query->exec(deviceinfo);
	QMap <QString, long> whatmap;
	whatmap.clear();
	while (db_query->next())
	{
		if (1)//db_query->value(5).toString().isEmpty())
		{
			long reLogin = loginFunction(db_query->value(0).toString().toLatin1().data(), db_query->value(1).toString().toLatin1().data(), db_query->value(2).toString().toLatin1().data(), db_query->value(3).toString().toLatin1().data(), db_query->value(4).toString().toLatin1().data());
			if (reLogin!=-1)
			{
				QString abc = db_query->value(0).toString();
				whatmap.insert(db_query->value(0).toString(), reLogin);
				//WriteDatabase(db_query->value(0).toString().toLatin1().data(), reLogin);
			}
		}
	}
	QMap<QString, long>::iterator it;
	for (it = whatmap.begin(); it !=whatmap.end();++it)
	{
		WriteDatabase(it.key().toLatin1().data(),it.value());
	}
	whatmap.clear();
}
long MemoryManager::ReadDatabase_ID(char*ip)
{
	char deviceinfo[200];
	ZeroMemory(deviceinfo, 200);
	sprintf(deviceinfo, "select * from Device_Sheet where ip='%s'", ip);
	db_query->exec(deviceinfo);
	while (db_query->next())
	{
		return db_query->value(5).toString().toLong();
	}
	return -1;
}
void MemoryManager::ReadDatabase_Ty(char* ip, char* DevType)
{
	char deviceinfo[200];
	ZeroMemory(deviceinfo, 200);
	sprintf(deviceinfo, "select * from Device_Sheet where ip='%s'", ip);
	db_query->exec(deviceinfo);
	while (db_query->next())
	{
		QVariant ipvariant = db_query->value(4);
		QString ipstring = ipvariant.toString();
		strncpy(DevType,(const char*)(ipstring.toLatin1().data()),2);
	}
}
void MemoryManager::GenerateSubmenus(void* qtwitem, long login_ID,char* DevType)
{
	int nerror;
	QTreeWidgetItem* subMenu = new QTreeWidgetItem((QTreeWidgetItem*)qtwitem);
	//QLabel* sdkNvr = new QLabel;
	//sdkNvr->setText(QString::fromLocal8Bit("通道名称"));
	//deviceInfo->setItemWidget(subMenu, 1, sdkNvr);
	subMenu->setText(0, QString((const char*)"10.9.11.5"));
	QLabel* image = new QLabel();
	QLabel* imageofvideo = new QLabel();
	QPixmap* movie = new QPixmap();
	QPixmap* movieofvideo = new QPixmap();
	movie->load(":/MemoryManager/Resources/wnd_button_bk/suc_tip.png");
	QPixmap Qmovie = movie->scaled(QSize(50, 20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	image->setPixmap(Qmovie);
	deviceInfo->setItemWidget(subMenu, 1, image);
	movieofvideo->load(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio.png");
	QPixmap Qmovieofvideo = movieofvideo->scaled(QSize(50, 20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	imageofvideo->setPixmap(Qmovieofvideo);
	deviceInfo->setItemWidget(subMenu, 2, imageofvideo);
	/*if (strncmp(DevType,"DH",2)==0)
	{
		DH_IN_MATRIX_GET_CAMERAS dhInMatrix = { 0 };
		dhInMatrix.dwSize = sizeof(DH_IN_MATRIX_GET_CAMERAS);
		DH_OUT_MATRIX_GET_CAMERAS dhOutMatrix = { 0 };
		dhOutMatrix.dwSize = sizeof(DH_OUT_MATRIX_GET_CAMERAS);
		dhOutMatrix.nMaxCameraCount = 128;
		dhOutMatrix.pstuCameras = new DH_MATRIX_CAMERA_INFO[dhOutMatrix.nMaxCameraCount];
		for (int i = 0; i < dhOutMatrix.nMaxCameraCount; i++)
		{
			dhOutMatrix.pstuCameras[i].dwSize = sizeof(DH_MATRIX_CAMERA_INFO);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.dwSize = sizeof(DH_REMOTE_DEVICE);
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount = 128;
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs = new DH_VIDEO_INPUTS[dhOutMatrix.pstuCameras[i].stuRemoteDevice.nMaxVideoInputCount];
			dhOutMatrix.pstuCameras[i].stuRemoteDevice.pstuVideoInputs->dwSize = sizeof(DH_VIDEO_INPUTS);
		}
		if (CLIENT_MatrixGetCameras(login_ID, &dhInMatrix, &dhOutMatrix, 5000))//获取前端摄像机列表信息
		{
			for (int i = 0; i < dhOutMatrix.nRetCameraCount; i++)
			{
				QTreeWidgetItem* subMenu = new QTreeWidgetItem((QTreeWidgetItem*)qtwitem);
				subMenu->setText(0,QString((const char*)dhOutMatrix.pstuCameras[i].stuRemoteDevice.szIp));
			}
		}
	    //nerror = CLIENT_GetLastError()&(0x7fffffff);
	}else if (strncmp(DevType,"HK",2)==0)
	{
		NET_DVR_IPPARACFG_V40 hkIP = { 0 };
		DWORD Hkre;
		if (NET_DVR_GetDVRConfig(login_ID, NET_DVR_GET_IPPARACFG_V40, 0, &hkIP, sizeof(NET_DVR_IPPARACFG_V40), &Hkre))
		{
			for (int i = 0; i <64; i++)
			{
				QTreeWidgetItem* subMenu = new QTreeWidgetItem((QTreeWidgetItem*)qtwitem);
				subMenu->setText(0,QString((const char*)(hkIP.struIPDevInfo[i].struIP.sIpV4)));
			}
		}
	}*/
}
void MemoryManager::GetState(CustomQlabel* image, char* ip)
{
	char deviceinfo[200];
	ZeroMemory(deviceinfo, 200);
	//image->ImageLocal("E:\\MemoryManager\\Win32\\Debug\\online.jpg");
	image->setText(QString::fromLocal8Bit("在线"));
	/*sprintf(deviceinfo, "select * from Device_Sheet where ip='%s'", ip);
	db_query->exec(deviceinfo);
	if (db_query->next())
	{
		DWORD stOnline;
		int reOnline;
		if (strncmp(db_query->value(4).toString().toLatin1().data(),"DH",2)==0)
		{
			if (CLIENT_QueryDevState(db_query->value(5).toString().toLong(), DH_DEVSTATE_ONLINE,(char*)&stOnline,sizeof(DWORD),&reOnline))
			{
				if (stOnline)
				{
					image->ImageLocal("E:/MemoryManager/Win32/Debug/online.jpg");
				}else {
					image->ImageLocal("E:/MemoryManager/Win32/Debug/offline.jpg");
				}
			}
		}else if(strncmp(db_query->value(4).toString().toLatin1().data(),"HK",2)==0)
		{

		}
	}*/
}
long MemoryManager::deleteData(char* ip)
{
	char deviceinfo[200];
	ZeroMemory(deviceinfo, 200);
	if (ip == NULL)
	{
		return 0;
	}
	else {
		sprintf(deviceinfo, "DELETE from Device_Sheet where ip='%s'", ip);
	}
	if (db_query->exec(deviceinfo))
	{
		QMessageBox::about(NULL, QString::fromLocal8Bit("设备管理"), QString::fromLocal8Bit("删除成功"));
		return 1;
	}
}
long MemoryManager::StatusofIP(char* ip)
{
	char deviceinfo[100];
	ZeroMemory(deviceinfo, 100);
	if (ip == NULL)
	{
		sprintf(deviceinfo, "select * from Device_Sheet");
	}
	else {
		sprintf(deviceinfo, "select * from Device_Sheet where ip='%s'", ip);
	}
	db_query->exec(deviceinfo);
	deviceInfo->clear();
	while (db_query->next())
	{
		QTreeWidgetItem *imageItem1 = new QTreeWidgetItem(deviceInfo, QStringList(db_query->value(0).toString()));
		QLabel* image = new QLabel();
		QLabel* imageofvideo = new QLabel();
		QLabel* memoryDisk = new QLabel();
		QToolButton* managedStorage = new QToolButton();
		managedStorage->setToolButtonStyle(Qt::ToolButtonIconOnly);
		QAction* manageAction = new QAction(managedStorage);
		manageAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/device.png"));
		managedStorage->setIconSize(QSize(100, 50));
		managedStorage->setDefaultAction(manageAction);
		managedStorage->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");

		QToolButton* Reping = new QToolButton();
		Reping->setToolButtonStyle(Qt::ToolButtonIconOnly);
		QAction* RepingAction = new QAction(Reping);
		RepingAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio.png"));
		Reping->setIconSize(QSize(25, 25));
		Reping->setDefaultAction(RepingAction);
		Reping->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");

		QToolButton* VideoStorage = new QToolButton();
		VideoStorage->setToolButtonStyle(Qt::ToolButtonIconOnly);
		QAction* VideoAction = new QAction(VideoStorage);
		VideoAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/timg.jpg"));
		VideoStorage->setIconSize(QSize(25, 25));
		VideoStorage->setDefaultAction(VideoAction);
		VideoStorage->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
		QToolButton* videoMemory = new QToolButton();
		videoMemory->setToolButtonStyle(Qt::ToolButtonIconOnly);
		QAction* vmAction = new QAction(videoMemory);
		vmAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/record_vedio_playback.png"));
		videoMemory->setIconSize(QSize(25, 25));
		videoMemory->setDefaultAction(vmAction);
		videoMemory->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");

		QPixmap* movie = new QPixmap();
		QPixmap* movieofvideo = new QPixmap();
		long shiklogID = db_query->value(5).toString().toLong();
		QString DeviceType = db_query->value(4).toString();
		if (DeviceType.left(2)==QString("DH"))
		{
			DWORD Status;
			int nRelen;
			if (CLIENT_QueryDevState(shiklogID, DH_DEVSTATE_ONLINE, (char*)&Status, sizeof(Status), &nRelen))
			{
				if (1 == Status)
				{
					movie->load(":/MemoryManager/Resources/wnd_button_bk/suc_tip.png");
				}
				else
				{
					movie->load(":/MemoryManager/Resources/wnd_button_bk/suc_tip_player.png");
				}
			}
		}
		else {
			if (NET_DVR_RemoteControl(shiklogID, NET_DVR_CHECK_USER_STATUS, NULL, sizeof(NULL)))
			{
				movie->load(":/MemoryManager/Resources/wnd_button_bk/suc_tip.png");
			}
			else
			{
				movie->load(":/MemoryManager/Resources/wnd_button_bk/suc_tip_player.png");
			}
		}
		QPixmap Qmovie = movie->scaled(QSize(25, 25), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		image->setPixmap(Qmovie);
		deviceInfo->setItemWidget(imageItem1, 1, image);
		//NET_DVR_WORKSTATE_V30 chanStatus;
		//DWORD reSize;
		//movieofvideo->load(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio_ing.png");
		//if (NET_DVR_GetDVRWorkState_V30(shiklogID,&chanStatus))
		//{
		//	for (int i=0;i<64;i++)
		//	{
		//		if (chanStatus.struChanStatic[i].byRecordStatic == 1)
		//		{
		//			movieofvideo->load(":/MemoryManager/Resources/wnd_button_bk/player/cut_vedio.png");
		//			break;
		//		}
		//	}
		//}
		//else {
		//	//QMessageBox::information(NULL, QString::fromLocal8Bit("设备状态提示"), QString::fromLocal8Bit("获取录像信息失败"));
		//}
		QPixmap Qmovieofvideo = movieofvideo->scaled(QSize(25, 25), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		imageofvideo->setPixmap(Qmovieofvideo);
		NET_DVR_HDCFG hdcfg;
		float hidkNumber=0;
		float hkFreeSpace = 0;
		DWORD reSize;
		if (strncmp(DeviceType.toLatin1().data(),"DH",2)==0)
		{
			DH_HARDDISK_STATE pBuf = { 0 };
			int pRetLen = 0;
			if (CLIENT_QueryDevState(shiklogID, DH_DEVSTATE_DISK, (char*)&pBuf, sizeof(DH_HARDDISK_STATE), &pRetLen))
			{
				for (int i = 0; i < pBuf.dwDiskNum; i++)
				{
					hkFreeSpace += pBuf.stDisks[i].dwFreeSpace;
					hidkNumber += pBuf.stDisks[i].dwVolume;
				}
			}
		}
		else {
			if (NET_DVR_GetDVRConfig(shiklogID, NET_DVR_GET_HDCFG_V40, 0, &hdcfg, sizeof(NET_DVR_HDCFG), &reSize))
			{
				for (int i = 0; i < 33 && i < hdcfg.dwHDCount; i++)
				{
					hidkNumber += hdcfg.struHDInfo[i].dwCapacity;
					hkFreeSpace += hdcfg.struHDInfo[i].dwFreeSpace;
				}
				if (hdcfg.dwHDCount > 33)
				{
					memset(&hdcfg, 0, sizeof(NET_DVR_HDCFG));
					if (NET_DVR_GetDVRConfig(shiklogID, NET_DVR_GET_HDCFG_V40, 1, &hdcfg, sizeof(NET_DVR_HDCFG), &reSize))
					{
						for (int i = 0; i < 33 && i < (hdcfg.dwHDCount - 33); i++)
						{
							hidkNumber += hdcfg.struHDInfo[i].dwCapacity;
							hkFreeSpace += hdcfg.struHDInfo[i].dwFreeSpace;
						}
					}
				}
			}
		}
		hkFreeSpace /=1024;
		hidkNumber /=1024;
		QString sNumber= QString("%1%2%3%4").arg(hkFreeSpace).arg("/").arg(hidkNumber).arg("(G)");
		memoryDisk->setText(sNumber);
		managedStorage->setText(QString::fromLocal8Bit("通道管理"));
		VideoStorage->setText(QString::fromLocal8Bit("录像管理"));
		videoMemory->setText(QString::fromLocal8Bit("录像回放"));
		connect(managedStorage, SIGNAL(clicked()), this, SLOT(AnthorDeviceInface()));
		connect(VideoStorage, SIGNAL(clicked()), this, SLOT(videoInface()));
		connect(videoMemory, SIGNAL(clicked()), this, SLOT(replay()));
		connect(Reping, SIGNAL(clicked()), this, SLOT(RepingFunc()));
		deviceInfo->setItemWidget(imageItem1, 2, Reping);// imageofvideo);
		deviceInfo->setItemWidget(imageItem1, 3, memoryDisk);
		deviceInfo->setItemWidget(imageItem1, 4, managedStorage);
		deviceInfo->setItemWidget(imageItem1, 5, VideoStorage);
		deviceInfo->setItemWidget(imageItem1, 6, videoMemory);
	}
	//QMessageBox::about(NULL, QString::fromLocal8Bit("设备状态"), QString::fromLocal8Bit("刷新结束"));
	refreshButton->setEnabled(true);
	return 0;
}
void MemoryManager::AnthorDeviceInface()
{
	QToolButton *button = dynamic_cast<QToolButton *>(QObject::sender()); //到信号发送者
	QModelIndex index = deviceInfo->indexAt(button->pos());
	int row = index.row();
	int column = index.column();
	QTreeWidgetItemIterator it(deviceInfo, QTreeWidgetItemIterator::All);
	it += row;
	QString ipstring = (*it)->text(0);
	char deviceinfo[200];
	ZeroMemory(deviceinfo, 200);
	sprintf(deviceinfo, "select * from Device_Sheet where ip='%s'", ipstring.toLatin1().data());
	long loginID;
	if (db_query->exec(deviceinfo))
	{
		while (db_query->next())
		{
			loginID = db_query->value(5).toString().toLong();
			this->hide();
			DeviceVideo* sadp = new DeviceVideo(this,(*it)->text(0).toLatin1().data(),db_query->value(4).toByteArray().data(),loginID);
			sadp->move(x, y);
			connect(sadp, SIGNAL(parentshow_s()), this, SLOT(reshow()));
		}
	}
}
void MemoryManager::videoInface()
{
	QToolButton *button = dynamic_cast<QToolButton *>(QObject::sender()); //到信号发送者
	QModelIndex index = deviceInfo->indexAt(button->pos());
	int row = index.row();
	int column = index.column();
	QTreeWidgetItemIterator it(deviceInfo, QTreeWidgetItemIterator::All);
	it += row;
	QString ipstring = (*it)->text(0);
	char deviceinfo[200];
	ZeroMemory(deviceinfo, 200);
	sprintf(deviceinfo, "select * from Device_Sheet where ip='%s'", ipstring.toLatin1().data());
	long loginID;
	if (db_query->exec(deviceinfo))
	{
		while (db_query->next())
		{
			loginID = db_query->value(5).toString().toLong();
			this->hide();
			ManagerInterface* inface = new ManagerInterface(this,(*it)->text(0).toLatin1().data(), db_query->value(4).toByteArray().data(), loginID);
			inface->move(x, y);
			connect(inface, SIGNAL(parentshow_s()), this, SLOT(reshow()));
		}
	}
}
void MemoryManager::replay()
{
	QToolButton *button = dynamic_cast<QToolButton *>(QObject::sender()); //到信号发送者
	QModelIndex index = deviceInfo->indexAt(button->pos());
	int row = index.row();
	int column = index.column();
	QTreeWidgetItemIterator it(deviceInfo, QTreeWidgetItemIterator::All);
	it += row;
	QString ipstring = (*it)->text(0);
	char deviceinfo[200];
	ZeroMemory(deviceinfo, 200);
	sprintf(deviceinfo, "select * from Device_Sheet where ip='%s'", ipstring.toLatin1().data());
	long loginID;
	if (db_query->exec(deviceinfo))
	{
		while (db_query->next())
		{
			loginID = db_query->value(5).toString().toLong();
			this->hide();
			MemroyPlay* oldVideo = new MemroyPlay(this, ipstring.toLocal8Bit().data(), db_query->value(4).toByteArray().data(), loginID);
			oldVideo->move(x, y);
			connect(oldVideo, SIGNAL(parentshow_s()), this, SLOT(reshow()));
		}
	}
}
void MemoryManager::beforeClose()
{
	if (db_query->exec("select * from Device_Sheet"))
	{
		while (db_query->next())
		{
			NET_DVR_Logout_V30(db_query->value(5).toInt());
		}
	}
	if (!db_query->exec("update Device_Sheet set loginID=''"))
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("数据库提示"), QString::fromLocal8Bit("清除登录ID失败"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	this->close();
}
void MemoryManager::reshow()
{
	this->show();
}
void MemoryManager::RepingFunc()
{
	QToolButton *button = dynamic_cast<QToolButton *>(QObject::sender()); //到信号发送者
	QModelIndex index = deviceInfo->indexAt(button->pos());
	int row = index.row();
	int column = index.column();
	QTreeWidgetItemIterator it(deviceInfo, QTreeWidgetItemIterator::All);
	it += row;
	QString ipstring = (*it)->text(0);
	char deviceinfo[200];
	ZeroMemory(deviceinfo, 200);
	sprintf(deviceinfo, "select * from Device_Sheet where ip='%s'", ipstring.toLatin1().data());
	long loginID;
	if (db_query->exec(deviceinfo))
	{
		while (db_query->next())
		{
			loginID = db_query->value(5).toString().toLong();
			this->hide();
			RepingFace* oldVideo = new RepingFace( ipstring.toLocal8Bit().data(), loginID,db_query->value(4).toByteArray().data(),this);
			oldVideo->move(x, y);
			connect(oldVideo, SIGNAL(parentshow_s()), this, SLOT(reshow()));
		}
	}
}
void MemoryManager::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastmousePosition = event->globalPos();
	}
}
void MemoryManager::mouseMoveEvent(QMouseEvent* event)
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
void MemoryManager::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
	}
}
void MemoryManager::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.drawRect(QRect(1, 1, WIDTHS - 2, HIGTHS - 2));
}