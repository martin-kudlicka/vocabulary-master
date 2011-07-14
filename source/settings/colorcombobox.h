#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QtGui/QComboBox>

class ColorComboBox : public QComboBox
{
	public:
		ColorComboBox(QWidget *pParent = 0);

	private:
		static const int BORDER_WIDTH = 2;

		void paintEvent(QPaintEvent * e);
}; // ColorComboBox

#endif // COLORCOMBOBOX_H