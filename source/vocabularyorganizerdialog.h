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

#ifndef TRY
		virtual void accept();
#endif
#ifdef FREE
		const void EnableControls() const;
#endif
#ifndef TRY
		const QString GetOpenPath() const;
		virtual void reject();
#endif

	private slots:
		const void on_qpbClose_clicked(bool checked = false);
		const void on_qpbNew_clicked(bool checked = false);
#ifndef TRY
		const void on_qpbOpen_clicked(bool checked = false);
#endif
		const void on_qtvVocabulariesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
}; // VocabularyOrganizerDialog

#endif // VOCABULARYORGANIZERDIALOG_H