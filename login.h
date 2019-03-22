#pragma once
#ifndef LOGIN_H
#define LOGIN_H
#include <QDialog>
class QLineEdit;
class QLabel;
class QToolButton;
class Login :public QDialog
{
	Q_OBJECT
public:
	Login(QWidget *parent = 0);
	~Login();
protected:
	QLabel* IpLabel;
	QLabel* NportLabel;
	QLabel* AdminLabel;
	QLabel* PasswdLabel;
	QLineEdit* IpLine;
	QLineEdit* NportLine;
	QLineEdit* AdminLine;
	QLineEdit* PasswdLine;
	QToolButton* SureButton;
	QToolButton* CancelButton;
signals:
	void SureSignal(QString ips,QString nports,QString admins,QString pas);
	void CancelSignal();
	void parentshow_s();
private slots:
	void CancelSlot();
	void SureSlot();
	void succed();
protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent* ev);
private:
	QPoint mLastmousePosition;
	bool mMoving;
};
#endif //LOGIN_H