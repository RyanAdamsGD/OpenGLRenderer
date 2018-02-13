#ifndef FLOAT_ELEMENT
#define FLOAT_ELEMENT
#include "PropertyElement.h"
#include <QtGui\qlineedit.h>

class FloatElement:public PropertyElement
{
	Q_OBJECT

	float* data;
	QLineEdit lineEdit;
		
public slots:
	void setData(float& value);
	void valueChanged(const QString& text);
public:
	FloatElement(void);
	~FloatElement(void){}
};

#endif