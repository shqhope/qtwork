#include "svgwidget.h"

SvgWidget::SvgWidget(QWidget *parent) : QSvgWidget(parent)
{
	// 构造函数获得本窗体的QSvgRenderer对象
	render = renderer();
}

void SvgWidget::wheelEvent(QWheelEvent *event)
{
	// 定义一个值，表示每次滚轮滚动的值，图片大小改变的比例
	const double diff = 0.1;
	// 先获取图片显示区的大小，以便进行下一步操作
	QSize size = render->defaultSize();
	int width = size.width();
	int height = size.height();

	// delta()函数获得滚轮滚动的距离值，返回值大于零是向前滚动
	if (event->delta() > 0)
	{
		width = int(this->width() + this->width() * diff);
		height = int(this->height() + this->height() * diff);
	}
	else
	{
		width = int(this->width() - this->width() * diff);
		height = int(this->height() - this->height() * diff);
	}

	resize(width, height);
}
