#include "svgwindow.h"

SvgWindow::SvgWindow(QWidget *parent)
	: QScrollArea(parent)
{
	svgWidget = new SvgWidget;
	setWidget(svgWidget);
}

// 当主窗口中对文件进行了选择和修改时，将调用setFile()函数设置新的文件
void SvgWindow::setFile(QString fileName)
{
	if (fileName == "")
		return;

	svgWidget->load(fileName);
	QSvgRenderer *render = svgWidget->renderer();
	svgWidget->resize(render->defaultSize());
}

void SvgWindow::mousePressEvent(QMouseEvent *event)
{
	mousePressPos = event->pos();
	// 获取水平滑动条和垂直滑动条
	scrollBarValueOnMousePress.rx() = horizontalScrollBar()->value();
	scrollBarValueOnMousePress.ry() = verticalScrollBar()->value();
	event->accept();
}

void SvgWindow::mouseMoveEvent(QMouseEvent *event)
{
	horizontalScrollBar()->setValue(scrollBarValueOnMousePress.x() - event->pos().x() + mousePressPos.x());
	verticalScrollBar()->setValue(scrollBarValueOnMousePress.y() - event->pos().y() + mousePressPos.y());
	horizontalScrollBar()->update();
	verticalScrollBar()->update();
	event->accept();
}
