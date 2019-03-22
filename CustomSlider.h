#pragma once
#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H
#include <QSlider>
#include "HCNetSDK.h"
class CustomRectItem :public QSlider
{
	Q_OBJECT
	public:
		CustomRectItem(QWidget *parent = NULL);
		//void paintEvent(QPaintEvent *ev);
		void setPlayback(LONG m_pl);
		void relesePlayback();
signals:
		void CustomRectItemMousePress();

protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	private:
		bool m_bResizing;
		LONG m_PlaybackControl;
};
#endif // !CUSTOMSLIDER_H
