#ifndef SVGWIDGET_H
#define SVGWIDGET_H

// svgwidget主要用于显示SVG图片
#include <QSvgWidget>
#include <QtSvg>
#include <QSvgRenderer>
#include <QWheelEvent>

class SvgWidget : public QSvgWidget
{
	Q_OBJECT
public:
	SvgWidget(QWidget *parent = 0);

	// 响应鼠标滚轮事件，使SVG图片能够通过鼠标滚轮的滚动改变图片大小
	void wheelEvent(QWheelEvent *);

private:
	// 用于图片显示尺寸的确定
	QSvgRenderer *render;
};

#endif
