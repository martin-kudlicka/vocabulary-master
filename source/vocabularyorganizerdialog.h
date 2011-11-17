#ifndef VOCABULARYORGANIZERDIALOG_H
#define VOCABULARYORGANIZERDIALOG_H

#include <ui_vocabularyorganizerdialog.h>

#include "vocabularyorganizerdialog/vocabularyorganizermodel.h"

class VocabularyOrganizerDialog : public QDialog
{
    Q_OBJECT

    public:
        VocabularyOrganizerDialog(const VocabularyOrganizer *pOrganizer, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		Ui::qdVocabularyOrganizer _qdvmOrganizer;
		const VocabularyOrganizer *_voOrganizer;
		VocabularyOrganizerModel _vomModel;
}; // VocabularyOrganizerDialog

#endif // VOCABULARYORGANIZERDIALOG_H