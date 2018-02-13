#include "QtPaintWindow.h"
#include "Qt\qpainter.h"
#include "Qt\qevent.h"


QtPaintWindow::QtPaintWindow(QWidget* widget)
	:QWidget(widget), gridScale(10), originX(0), originY(0),originOffsetX(0),
	originOffsetY(0),circleList(QVector<circle>()),lineList(QVector<line>()),
	currentParticle(NULL)
{
	useAdditiveLines = true;
}

void QtPaintWindow::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing,true);
	painter.drawEllipse(QPoint(500,0),20,20);
	drawBackground();
	drawGrid();
	for(int i=0;i<circleList.count();i++)
	{
		circle& c = circleList[i];
		drawCircle(c.position->x,c.position->y,*c.scale,*c.color);
	}
	for(int i=0;i<lineList.count();i++)
	{
		line& l = lineList[i];
		drawLine(glm::vec2(l.start->x,l.start->y),glm::vec2(l.end->x,l.end->y),*l.color);
	}
}

void QtPaintWindow::drawBackground()
{
	QPainter painter(this);
	painter.setBrush(QBrush(QColor::fromRgb(255,255,255)));
	painter.drawRect(0,0,this->width()-BORDER,this->height()-BORDER);	
}

void QtPaintWindow::drawGrid()
{
	QPainter painter(this);
	painter.setBrush(QBrush(QColor::fromRgb(0,0,0)));
	QVector<QPoint> horizontal;
	QVector<QPoint> vertical;
	for(int i=1; i< this->height()/verticalSpacing;i++)
	{
		horizontal.push_back(QPoint(0,verticalSpacing * i));
		horizontal.push_back(QPoint(this->width()-BORDER,verticalSpacing * i));
	}
	for(int i=1; i< this->width()/horizontalSpacing;i++)
	{
		vertical.push_back(QPoint(horizontalSpacing*i, 0));
		vertical.push_back(QPoint(horizontalSpacing * i,this->height()-BORDER));
	}
	painter.drawLines(horizontal);
	painter.drawLines(vertical);
	//draw main axis
	painter.setPen(QPen(QBrush(QColor::fromRgb(0,0,0)),4));
	if(horizontal.count()>0)
	{
		originY = horizontal[horizontal.count()/2-2].y() + originOffsetX;
		painter.drawLine(QPoint(0,originY),QPoint(this->width()-BORDER,originY));
	}
	if(vertical.count()>0)
	{
		originX = vertical[vertical.count()/2-2].x() + originOffsetY;
		painter.drawLine(QPoint(originX,0),QPoint(originX, this->height()-BORDER));
	}
}

void QtPaintWindow::updateSpacing()
{
	verticalSpacing = (this->height()-BORDER)/gridScale;
	horizontalSpacing = (this->width()-BORDER)/gridScale;
}

void QtPaintWindow::update()
{
	updateSpacing();
}

void QtPaintWindow::drawCircle(float X, float Y, float scale, const glm::vec4& color)
{
	scale = 1/scale;
	QPainter painter(this);
	painter.setBrush(QBrush(QColor::fromRgb(color.x, color.y,color.z)));
	QPoint position(originX + X * horizontalSpacing,originY + Y * -verticalSpacing);
	painter.drawEllipse(position,(int)(scale*horizontalSpacing *0.025),(int)(scale*verticalSpacing*0.025));
}

void QtPaintWindow::drawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
{
	QPainter painter(this);
	painter.setPen(
		QPen(QBrush(QColor(color.x,color.y,color.z)), 3));
	QPoint begin(originX + (start.x * horizontalSpacing), originY + (start.y * - verticalSpacing));

	QPoint finish;

	if(useAdditiveLines)
		finish = QPoint(originX + ((end.x + start.x) * horizontalSpacing), originY + ((end.y + start.y) * - verticalSpacing));
	else
		finish = QPoint(originX + ((end.x) * horizontalSpacing), originY + ((end.y) * - verticalSpacing));

	painter.drawLine(begin, finish);

	//// Draw the arrow tip
	//float baseAngle = atan2((float)finish.y() - begin.y(),
	//	(float)finish.x() - begin.x());

	//const float EDGE_LENGTH = 15; // Pixels
	//const float PI = 3.1415927f;
	//const float OFFSET_ANGLE = 3 * (PI / 4);

	//// The tip is the pivot point for the edges
	//float leftEdgeAngle = baseAngle + OFFSET_ANGLE;
	//painter.drawLine(finish,
	//	QPoint(finish.x() + cos(leftEdgeAngle) * EDGE_LENGTH,
	//	finish.y() + sin(leftEdgeAngle) * EDGE_LENGTH));

	//float rightEdgeAngle = baseAngle - OFFSET_ANGLE;
	//painter.drawLine(finish,
	//	QPoint(finish.x() + cos(rightEdgeAngle) * EDGE_LENGTH,
	//	finish.y() + sin(rightEdgeAngle) * EDGE_LENGTH));
}


void QtPaintWindow::mouseMoveEvent(QMouseEvent* e)
{
	if(currentParticle != NULL)
	{
		QPointF cursorPos = e->posF();
		currentParticle->x = (-originX + cursorPos.x()) / horizontalSpacing;
		currentParticle->y = (-originY + cursorPos.y()) / -verticalSpacing;
	}
}

void QtPaintWindow::mousePressEvent(QMouseEvent* e)
{
	if(currentParticle == NULL)
	{
		for(uint i=0;i<circleList.count();i++)
		{
			QPointF cursorPos = e->posF();

			QPoint position(originX + circleList[i].position->x * horizontalSpacing,originY + circleList[i].position->y * -verticalSpacing);
			if(glm::length(glm::vec2(cursorPos.x() - position.x(),cursorPos.y()-position.y())) < *circleList[i].scale + 25)
			{
				currentParticle = const_cast<glm::vec3*>(circleList[i].position);
			}
		}
	}
}

void QtPaintWindow::mouseReleaseEvent(QMouseEvent* e)
{
	currentParticle = NULL;
}


void QtPaintWindow::wheelEvent(QWheelEvent* e)
{
	if(e->delta() > 0)
	{
		gridScale += 2;
	}
	else if(e->delta() < 0 && gridScale > 10)
	{
		gridScale-=2;
	}
}

void QtPaintWindow::addParticle(const glm::vec3* position, const float* scale,const glm::vec4* color,const bool* showBoundBox)
{
		circleList.push_back(circle(position,scale,showBoundBox,color));
}


void QtPaintWindow::addLine(const glm::vec3* start, const glm::vec3* end, const glm::vec4* color)
{
	lineList.push_back(line(start,end,color));
}

void QtPaintWindow::clearParticleList()
{
	circleList.clear();
}

void QtPaintWindow::clearLineList()
{
	lineList.clear();
}
