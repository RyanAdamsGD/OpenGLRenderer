#ifndef PROPERTY_WINDOW
#define PROPERTY_WINDOW
#include <QtGui\qwidget.h>

class PropertyWindow: public QWidget
{
	Q_OBJECT

public:
	PropertyWindow(void);
	~PropertyWindow(void);
};

#endif