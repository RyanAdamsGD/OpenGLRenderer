#ifndef QTPAINTWINDOW_H
#define QTPAINTWINDOW_H
#include <QtGui\qwidget.h>
#include "IParticleDrawer.h"


class QtPaintWindow:public QWidget, public ParticleDrawer
{
	glm::vec3* currentParticle;
	static const int BORDER = 5;
	float verticalSpacing, horizontalSpacing;
	int originX, originY;
	int originOffsetX, originOffsetY;
	int gridScale;
	QPointF previousMousePos;

	void drawBackground();
	void drawGrid();
	void updateSpacing();
	void drawCircle(const float X, const float Y, const float scale, const glm::vec4& color);
	void drawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);

	struct circle
	{
		const glm::vec3* position;
		const float* scale;
		const bool* showBoundingBox;
		const glm::vec4* color;
		circle(const glm::vec3* position, const float* scale, const bool* showBoundingBox, const glm::vec4* color)
			:position(position),scale(scale),showBoundingBox(showBoundingBox), color(color){}
		circle():position(NULL),scale(NULL),showBoundingBox(NULL),color(NULL){}
	};
	QVector<circle> circleList;

	struct line
	{
		const glm::vec3* start;
		const glm::vec3* end;
		const glm::vec4* color;
		line(const glm::vec3* start, const glm::vec3* end, const glm::vec4* color)
			:start(start),end(end), color(color){}
		line():start(NULL),end(NULL), color(NULL){}
	};
	QVector<line> lineList;

public:
	virtual void addParticle(const glm::vec3* position, const float* scale = &DEFAULT_SCALE, const glm::vec4* color = &DEFAULT_COLOR,const bool* showBoundBox = &DEFAULT_SHOW);
	QtPaintWindow(QWidget* widget);
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void wheelEvent(QWheelEvent* e);
	void paintEvent(QPaintEvent* e);
	virtual void update();
	virtual void addLine(const glm::vec3* start, const glm::vec3* end, const glm::vec4* color = &DEFAULT_COLOR);
	virtual void clearParticleList();
	virtual void clearLineList();
};

#endif