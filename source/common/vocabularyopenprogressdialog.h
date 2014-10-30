#ifndef VOCABULARYOPENPROGRESSDIALOG_H
#define VOCABULARYOPENPROGRESSDIALOG_H

#include <ui_vocabularyopenprogressdialog.h>

#include <QtWidgets/QDialog>
#include "vocabulary.h"

class VocabularyOpenProgressDialog : public QDialog
{
	Q_OBJECT

	public:
		VocabularyOpenProgressDialog(const Vocabulary *pVocabulary, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		Ui::qdVocabularyOpenProgress _qdvopVocabularyOpenProgress;

	private slots:
		const void on_vVocabulary_SetOpenProgressValue(quint8 pValue) const;
		const void on_vVocabulary_SetOpenProgressMax(quint8 pMax) const;
		const void on_vVocabulary_SetVocabularyName(const QString &pName) const;
}; // VocabularyOpenProgressDialog

#endif // VOCABULARYOPENPROGRESSDIALOG_H