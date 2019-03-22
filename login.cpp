#include "login.h"
#include "MemoryManager.h"
#include "WindowTitle.h"
#include <QPoint>
#include <windows.h>
#include <QGridLayout>
#include <QToolButton>
#include <QMouseEvent>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>
#include <QWidget>
#include <QPainter>
#define LWINDTH 250
#define LHIGHT 270

Login::Login(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	/*setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setWindowModality(Qt::WindowModal);*/
	setModal(true);
	/*WindowTitle* wTitle = new WindowTitle(this, LWINDTH, 99);
	wTitle->setGeometry(0, 0, LWINDTH, 99);
	wTitle->show();*/
	QToolButton* closeButton = new QToolButton(this);
	closeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* pActions = new QAction(closeButton);
	pActions->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/close_wnd_h.png"));
	closeButton->setIconSize(QSize(100, 50));
	closeButton->setDefaultAction(pActions);
	closeButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	closeButton->setGeometry(LWINDTH - 40, 0, 50, 30);
	closeButton->show();
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(CancelSlot()));

	this->setWindowTitle(QString::fromLocal8Bit("登录界面"));
	QPoint MousePoint = cursor().pos();
	this->setGeometry(MousePoint.x()+200, MousePoint.y(), LWINDTH, LHIGHT);
	IpLabel = new QLabel(this);
	IpLabel->setText(QString::fromLocal8Bit("IP"));
	NportLabel = new QLabel(this);
	NportLabel->setText(QString::fromLocal8Bit("端口"));
	AdminLabel = new QLabel(this);
	AdminLabel->setText(QString::fromLocal8Bit("用户名"));
	PasswdLabel = new QLabel(this);
	PasswdLabel->setText(QString::fromLocal8Bit("密码"));
	IpLine = new QLineEdit(this);
	//IpLine->setInputMask("000.000.000.000; ");
	QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
	IpLine->setValidator(new QRegExpValidator(rx,this));
	IpLine->setMaxLength(15);
	NportLine = new QLineEdit(this);
	NportLine->setValidator(new QIntValidator(NportLine));
	AdminLine = new QLineEdit(this);
	PasswdLine = new QLineEdit(this);
	PasswdLine->setEchoMode(QLineEdit::Password);
	IpLabel->setGeometry(10, 40, 50, 30);
	IpLine->setGeometry(70, 40, 120, 20);
	NportLabel->setGeometry(10, 90, 50, 30);
	NportLine->setGeometry(70, 90, 120, 20);
	AdminLabel->setGeometry(10, 140, 50, 30);
	AdminLine->setGeometry(70, 140, 120, 20);
	PasswdLabel->setGeometry(10, 190, 50, 30);
	PasswdLine->setGeometry(70, 190, 120, 20);
	QGridLayout* grid = new QGridLayout;
	grid->addWidget(IpLabel, 0, 0);
	grid->addWidget(IpLine, 0, 1);
	grid->addWidget(NportLabel, 1, 0);
	grid->addWidget(NportLine, 1, 1);
	grid->addWidget(AdminLabel, 2, 0);
	grid->addWidget(AdminLine, 2, 1);
	grid->addWidget(PasswdLabel, 3, 0);
	grid->addWidget(PasswdLine, 3, 1);
	SureButton = new QToolButton(this);
	SureButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* sureAction = new QAction(SureButton);
	sureAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/advance.png"));
	SureButton->setIconSize(QSize(80, 30));
	SureButton->setDefaultAction(sureAction);
	SureButton->setStyleSheet("color: rgb(52,52,52);background-color: transparent;");
	SureButton->setGeometry(150, 230, 80, 30);
	SureButton->show();
	
	/*CancelButton = new QToolButton(this);
	CancelButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QAction* CancelAction = new QAction(CancelButton);
	CancelAction->setIcon(QIcon(":/MemoryManager/Resources/wnd_button_bk/cancel.png"));
	CancelButton->setIconSize(QSize(80, 30));
	CancelButton->setDefaultAction((CancelAction));
	CancelButton->setStyleSheet("color:rgb(52,52,52);background-color:transparent;");
	CancelButton->setGeometry(150, 250, 80, 30);
	CancelButton->show();
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(CancelSlot()));*/
	connect(SureButton, SIGNAL(clicked()), this, SLOT(SureSlot()));
	this->show();
}
Login::~Login()
{
	this->hide();
	delete IpLabel;
	delete IpLine;
	delete NportLabel;
	delete NportLine;
	delete AdminLabel;
	delete AdminLine;
	delete PasswdLabel;
	delete PasswdLine;
	delete SureButton;
	//delete CancelButton;
}
void Login::CancelSlot()
{
	parentshow_s();
	CancelSignal();
}
void Login::SureSlot()
{
	if (IpLine->text().isEmpty()|NportLine->text().isEmpty()|AdminLine->text().isEmpty()|PasswdLine->text().isEmpty())
	{
		return;
	}
	SureSignal(this->IpLine->text(),this->NportLine->text(),this->AdminLine->text(),this->PasswdLine->text());
}
void Login::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastmousePosition = event->globalPos();
	}
}
void Login::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
	{
		if (ReleaseCapture())
			SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
		event->ignore();
		//this->move(this->pos() + (event->globalPos() - mLastmousePosition));
		//mLastmousePosition = event->globalPos();
	}
}
void Login::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
	}
}
void Login::succed()
{
	IpLine->clear();
	NportLine->clear();
	AdminLine->clear();
	PasswdLine->clear();
}
void Login::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.drawRect(QRect(1, 1, LWINDTH - 2, LHIGHT - 2));
}