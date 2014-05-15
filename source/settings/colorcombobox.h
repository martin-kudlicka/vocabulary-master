#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QtWidgets/QComboBox>

class ColorComboBox : public QComboBox
{
	public:
		ColorComboBox(QWidget *parent = NULL);

	private:
		static const quint8 BORDER_WIDTH = 2;

		virtual void paintEvent(QPaintEvent * e);
}; // ColorComboBox

#endif // COLORCOMBOBOX_H