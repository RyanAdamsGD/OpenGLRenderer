#ifndef PROPERTY_ELEMENT
#define PROPERTY_ELEMENT
#include <Qt\qobject.h>
#include <QtGui\qlabel.h>

class PropertyElement :public QObject
{
	Q_OBJECT

	QLabel propertyName;
public:
	virtual void setPropertyName(const QString& text) {propertyName.setText(text);}
};

#endif