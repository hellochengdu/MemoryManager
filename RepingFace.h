#pragma once
#include <QWidget>
class QTreeWidget;
class QPoint;
class QTimer;
class RepingFace: public QWidget
{
	Q_OBJECT
public: 
	RepingFace(char* ips, long loginID,char* deviceType, QWidget *parent = NULL);
protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent* ev);
signals:
	void parentshow_s();
private slots:
	void GetReping();
	void GetReping_id(int a);
	void parentshow();
	void startHandReping();
	void TimeingVideoFunc();
	void VideoStatus();
	void ManualFunc();
	void PlanFunc();
private:
	char* ip;
	char DeviceType[6];
	long longid;
	QPoint mLastmousePosition;
	bool mMoving;
	bool ManualSign;
	bool PlanLogo;
	QTreeWidget* RepWidget;
	void* HandReping;
	QTimer* VideoTime;
};