#ifndef VOCABULARYOPENPROGRESSWIDGET_H
#define VOCABULARYOPENPROGRESSWIDGET_H

#include <ui_vocabularyopenprogressdialog.h>

#include <QtGui/QDialog>
#include "vocabulary.h"

class VocabularyOpenProgressDialog : public QDialog
{
	Q_OBJECT

	public:
		VocabularyOpenProgressDialog(const Vocabulary *pVocabulary, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		Ui::qdVocabularyOpenProgress _qdvopVocabularyOpenProgress;

	private slots:
		const void on_vVocabulary_SetOpenProgressValue(const int &pValue) const;
		const void on_vVocabulary_SetOpenProgressMax(const int &pMax) const;
		const void on_vVocabulary_SetVocabularyName(const QString &pName) const;
}; // VocabularyOpenProgressDialog

#endif // VOCABULARYOPENPROGRESSWIDGET_H