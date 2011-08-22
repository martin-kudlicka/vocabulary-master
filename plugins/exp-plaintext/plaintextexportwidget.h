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

    signals:
        void VocabularyGetMarks(QStringList *pMarks) const;

    private slots:
        const void on_qpbPlainRefresh_clicked(bool checked = false) const;
}; // PlaintextExportWidget

#endif // PLAINTEXTEXPORTWIDGET_H