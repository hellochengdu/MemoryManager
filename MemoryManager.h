#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MemoryManager.h"
class QSqlDatabase;
class QSqlQuery;
class QTreeWidget;
class QTreeWidgetItem;
class CustomQlabel;
class QToolButton;
class MemoryManager : public QMainWindow
{
    Q_OBJECT

public:
    MemoryManager(QWidget *parent = Q_NULLPTR);
	long loginFunction(char* ip, char* nport, char* adm, char*psd,char* Dev=NULL);//登录函数
	void ConnectDatabase();//连接数据库函数
	void WriteDatabase(char* ip, long LoginID,int nport=0,char* adm=NULL,char* psd=NULL,char*Dev=NULL);//写数据
	void ReadDatabase(char* ip=NULL);//读数据
	long ReadDatabase_ID(char*ip);//读数据返回ID
	void ReadDatabase_Ty(char* ip, char* DevType);
	void GenerateSubmenus(void* qtwitem, long login_ID, char* DevType);
	void GetState(CustomQlabel* image,char* ip);
	long deleteData(char* ip);
	long StatusofIP(char* ip=NULL);
signals:
	void loginsuccess();
private slots: 
	void construction();
	void pushFction();
	void showSelectedImg();
	void managerInterface(QTreeWidgetItem*qtwitem, int num);
	void doubleFunc(QTreeWidgetItem*qtwitem, int num);
	void CancelSlot();
	void SureSlot(QString ips, QString nports, QString admins, QString pas);
	void AnthorDeviceInface();
	void videoInface();
	void replay();
	void beforeClose();
	void reshow();
	void RepingFunc();
protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent* ev);
private:
	QPoint mLastmousePosition;
	bool mMoving;
	QSqlDatabase* db;
	QSqlQuery* db_query;
	QTreeWidget* deviceInfo;
	QTreeWidget* deviceInfo_state;
	QToolButton* addButton;
	QToolButton* pushButton;
	QToolButton* refreshButton;
	int x;
	int y;
    Ui::MemoryManagerClass ui;
};
