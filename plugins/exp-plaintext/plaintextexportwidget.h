#ifndef PLAINTEXTEXPORTWIDGET_H
#define PLAINTEXTEXPORTWIDGET_H

#include <ui_plaintextexportwidget.h>

class PlaintextExportWidget : public QWidget
{
	Q_OBJECT

	public:
		PlaintextExportWidget(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		Ui::qwPlaintextExport _qwpePlaintextExport;
}; // PlaintextExportWidget

#endif // PLAINTEXTEXPORTWIDGET_H