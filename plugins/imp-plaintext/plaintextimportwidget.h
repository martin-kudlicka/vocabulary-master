#ifndef PLAINTEXTIMPORTWIDGET_H
#define PLAINTEXTIMPORTWIDGET_H

#include <ui_plaintextimportwidget.h>

#include "../common/codecsmodel.h"
#include "plaintextimportwidget/plaintextfile.h"

class PlaintextImportWidget : public QWidget
{
    Q_OBJECT

    public:
        PlaintextImportWidget(PlaintextFile *file, QWidget *parent = NULL, Qt::WindowFlags flags = 0);

		const quint16 lineCount     () const;
		const quint16 linesPerRecord() const;
		const QString regExp        () const;

    private:
        CodecsModel               _codecsModel;
		PlaintextFile            *_file;
        Ui::PlaintextImportWidget _ui;

		virtual ~PlaintextImportWidget();

		const void preselectCodec() const;
		const void refreshPreview() const;

	private slots:
		const void on_codecs_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
		const void on_linesPerRecord_valueChanged           (int i)                                                            const;
}; // PlaintextImportWidget

#endif // PLAINTEXTIMPORTWIDGET_H