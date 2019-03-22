#include "CustomSlider.h"
#include <QDebug>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include "HCNetSDK.h"
CustomRectItem::CustomRectItem(QWidget *parent /* = 0 */)
{
	this->setStyleSheet("QSlider::groove:horizontal{ \
                                                height: 10px; \
                                                left: 5px; \
                                                right: 5px; \
                                                background: #dcdcdc; \
                                              } \
                          QSlider::handle:horizontal{ \
                                                border-radius: 10px; \
                                                width:  20px; \
                                                margin: -5px -1px; \
                                                background: green; } \
                          QSlider::sub-page:horizontal{background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(27, 5, 27, 255), stop:0.25 rgba(99, 20, 102, 255), stop:0.5 rgba(154, 30, 158, 255), stop:1 rgba(173, 57, 176, 255));}");
	this->setOrientation(Qt::Horizontal);
	m_PlaybackControl = -1;
	this->setMinimum(0);
	this->setMaximum(1000);
	this->setTickPosition(QSlider::TicksAbove);
	//connect(this, SIGNAL(mousePressEvent(QMouseEvent*)), this, SIGNAL(mouseslot(QMouseEvent*)));
	this->show();
}
void CustomRectItem::setPlayback(LONG m_pl)
{
	m_PlaybackControl = m_pl;
	return;
}
void CustomRectItem::relesePlayback()
{
	m_PlaybackControl = -1;
}
void CustomRectItem::mousePressEvent(QMouseEvent *ev)
{
	QSlider::mousePressEvent(ev);
	double pos = ev->pos().x() / (double)this->width();
	this->setValue(pos*(this->maximum() - this->minimum()) + minimum());
	emit CustomRectItemMousePress();
}
void CustomRectItem::mouseMoveEvent(QMouseEvent *ev)
{
	QSlider::mouseMoveEvent(ev);
}