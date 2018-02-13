#include "FloatElement.h"


FloatElement::FloatElement(void)
{
	connect(&lineEdit,SIGNAL(textEdited(const QString&)),this, SLOT(valueChanged(const QString&)));
}

void FloatElement::setData(float& value)
{
	*data = value;

	char* text;
	sprintf(text,"%f",value);
	lineEdit.setText(text);
}

void FloatElement::valueChanged(const QString& text)
{
	bool isFloat;
	text.toFloat(&isFloat);
	if(isFloat)
		*data = text.toFloat();
}