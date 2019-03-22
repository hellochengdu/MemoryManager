#pragma once
#ifndef CUSTOM_QLABEL_H
#define CUSTOM_QLABEL_H
#include <QLabel>
class CustomQlabel :public QLabel
{
	Q_OBJECT
public:
	void ImageLocal(char* path);
};
#endif