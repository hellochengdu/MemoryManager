#include "WindowTitle.h"
#include <QToolButton>
#include <QAction>
WindowTitle::WindowTitle(QWidget* parent,int width,int height)
{
	//this->resize(width, height);
	this->setParent(parent);
	this->resize(width, height);
	//设置背景图片
	this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QBrush(QPixmap(":/MemoryManager/Resources/wnd_button_bk/player/default_bk222.png").scaled(// 缩放背景图.
		this->size(),
	Qt::IgnoreAspectRatio,
	Qt::SmoothTransformation)));             // 使用平滑的缩放方式
	this->setPalette(palette);
	//隐藏标题栏
	this->setWindowFlags(Qt::FramelessWindowHint);
	//this->show();
}
void WindowTitle::closefalse()
{
	
}