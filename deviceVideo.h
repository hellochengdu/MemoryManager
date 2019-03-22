#pragma once
#ifndef DEVICEVIDEO_H
#define DEVICEVIDEO_H
#include <QtWidgets/QMainWindow>
#include <QMap>
class QTreeWidget;
class QSqlDatabase;
class QSqlQuery;
class QTreeWidgetItem;
class DeviceVideo:public QMainWindow
{
	Q_OBJECT
public:
	DeviceVideo(QWidget *parent = NULL, char* ipstring=NULL,/*char* user=NULL,char* passwd=NULL,*/ char* deviceType=NULL, long longID=0);
	long OSDFunc();
	void CloseFunc_hand();
signals:
	void parentshow_s();
private slots:
void addslot();
void deleteslot();
void doubleFunc(QTreeWidgetItem* item, int column);
void CloseFunc(QTreeWidgetItem* item, int column);
void currentItemChangedFunc(QTreeWidgetItem *current, QTreeWidgetItem *previous);
void parentshow();
protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent* ev);
private:
	char* ip;
	long longid;
	char DeviceType[6];
	char User[64];
	char Passwd[64];
	QPoint mLastmousePosition;
	QMap<QString ,QString> OSDmap;
	bool mMoving;
	QTreeWidget* ipcInface;
	QTreeWidgetItem* row;
};
#endif