#pragma once
#ifndef WINDOWTITLE_H
#define WINDOWTITLE_H
#include <QWidget>
class WindowTitle :public QWidget
{
	Q_OBJECT
public:
	WindowTitle(QWidget *parent = Q_NULLPTR,int width=0,int height=0);
	private slots:
	void closefalse();
};
#endif  //!WINDOWTITLE_H