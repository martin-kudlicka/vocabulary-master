#ifndef VOCABULARYORGANIZERDIALOG_H
#define VOCABULARYORGANIZERDIALOG_H

#include <ui_vocabularyorganizerdialog.h>

#include "vocabularyorganizerdialog/vocabularyorganizermodel.h"

class VocabularyOrganizerDialog : public QDialog
{
    Q_OBJECT

    public:
        VocabularyOrganizerDialog(VocabularyOrganizer *pOrganizer, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		Ui::qdVocabularyOrganizer _qdvmOrganizer;
		VocabularyOrganizer *_voOrganizer;
		VocabularyOrganizerModel _vomModel;

	private slots:
		const void on_qpbNew_clicked(bool checked = false);
		const void on_qtvVocabulariesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
}; // VocabularyOrganizerDialog

#endif // VOCABULARYORGANIZERDIALOG_H