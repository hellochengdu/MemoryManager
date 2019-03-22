#pragma once
#ifndef MANAGEMENTINTERFACE_H
#define MANAGEMENTINTERFACE_H
#include <QWidget>
class QStringList;
class QComboBox;
class QTimeEdit;
class ManagerInterface :public QWidget
{
	Q_OBJECT
public:
	ManagerInterface(QWidget *parent =NULL,char* ips=NULL,char* devicetype=NULL,long loginID=0);
	long GetIpNumber(QStringList* &ipList);
	void getvideo(QString ipcnum=NULL);
	void paintEvent(QPaintEvent *e);
	//~ManagerInterface();
protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
signals:
	void parentshow_s();
	private slots:
	void setvideo();
	void ChangedFunction(QString text);
	void parentshow();
private:
	QPoint mLastmousePosition;
	QComboBox* ipcnumber;
	QComboBox* allipc;
	QTimeEdit* sTime[7][8];
	QTimeEdit* eTime[7][8];
	char DeviceType[6];
	bool mMoving;
	char* ip;
	long longid;
};
#endif // !MANAGEMENTINTERFACE_H