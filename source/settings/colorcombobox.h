#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QtWidgets/QComboBox>

class ColorComboBox : public QComboBox
{
	public:
		ColorComboBox(QWidget *parent = nullptr);

	private:
		static const quint8 BORDER_WIDTH = 2;

		virtual ~ColorComboBox() override;

		virtual void paintEvent(QPaintEvent * e) override;
}; // ColorComboBox

#endif // COLORCOMBOBOX_H