#pragma once
#ifndef MEMORYPLAY_H
#define MEMORYPLAY_H
#include <QWidget>
#include "HCNetSDK.h"
class QDateTimeEdit;
class QTimer;
class QToolButton;
class QComboBox;
class QVideoWidget;
class CustomRectItem; 
class QTimer;
class QTreeWidget;
class QStringList;
class QTreeWidgetItem;
class MemroyPlay :public QWidget
{
	Q_OBJECT
public:
	MemroyPlay(QWidget *parent =NULL,char* ip=NULL,char* devicetype=NULL,long longid=0);
	private slots:
	void setTime(const QDate &date);
	void paintEvent(QPaintEvent *e);
	long GetIpNumbers(QStringList* &ipList);
protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
signals:
	void parentshow_s();
private slots:
void play();
void processOneThing();
void pauseFunc();
void fastFunc();
void slowFunc();
void playbackFunc();
void playbackfFunc();
void voideFunc();
void otherFunc();
void setPos();
void setvideoPos();
void parentshow();
void Timefunc();
void seekFunc();
void DoubleClickedFunc(QTreeWidgetItem* item, int column);
private:
	QPoint mLastmousePosition;
	bool mMoving;
	QDateTimeEdit* sTime;
	QDateTimeEdit* eTime;
	QTimer* doubleTimer;
	QVideoWidget* VideoWidget;
	QTreeWidget* fileWidget;
	QToolButton* pauseButton;
	QToolButton* fastButton;
	QToolButton* slowButton;
	//QToolButton* normalButton;
	QToolButton* playbacktrueButton;
	QToolButton* playbackfalseButton;
	QToolButton* voideButton;
	QToolButton* otherButton;
	CustomRectItem* playbackSlider;
	CustomRectItem* videoControl;
	QTimer* TimeSetting;
	char DeviceType[6];
	long logID;
	QComboBox* ipc;
	LONG m_lhPlayBack;
	bool playpauseState;
	bool playDirection;
	bool playVideo;
};
#endif // !MEMORYPLAY_H