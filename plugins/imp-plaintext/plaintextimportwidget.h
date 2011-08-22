#ifndef PLAINTEXTIMPORTWIDGET_H
#define PLAINTEXTIMPORTWIDGET_H

#include <ui_plaintextimportwidget.h>

#include "../common/codecsmodel.h"
#include "plaintextimportwidget/plaintextfile.h"

class PlaintextImportWidget : public QWidget
{
    Q_OBJECT

    public:
        PlaintextImportWidget(PlaintextFile *pFile, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

		const int GetLineCount() const;
		const int GetLinesPerRecord() const;
		const QString GetRegExp() const;

    private:
        CodecsModel _cmCodecsModel;
		PlaintextFile *_pfFile;
        Ui::qwPlaintextImport _qwpiPlaintextImport;

		const void PreselectCodec() const;
		const void RefreshPreview() const;

	private slots:
		const void on_qsbLinesPerRecord_valueChanged(int i) const;
		const void on_qtvCodecsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
}; // PlaintextImportWidget

#endif // PLAINTEXTIMPORTWIDGET_H