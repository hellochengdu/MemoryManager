#include "WindowTitle.h"
#include <QToolButton>
#include <QAction>
WindowTitle::WindowTitle(QWidget* parent,int width,int height)
{
	//this->resize(width, height);
	this->setParent(parent);
	this->resize(width, height);
	//���ñ���ͼƬ
	this->setAutoFillBackground(true); // ���Ҫ����, ���������ʾ��������ͼ.
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QBrush(QPixmap(":/MemoryManager/Resources/wnd_button_bk/player/default_bk222.png").scaled(// ���ű���ͼ.
		this->size(),
	Qt::IgnoreAspectRatio,
	Qt::SmoothTransformation)));             // ʹ��ƽ�������ŷ�ʽ
	this->setPalette(palette);
	//���ر�����
	this->setWindowFlags(Qt::FramelessWindowHint);
	//this->show();
}
void WindowTitle::closefalse()
{
	
}