#ifndef VOCABULARYOPENPROGRESSDIALOG_H
#define VOCABULARYOPENPROGRESSDIALOG_H

#include <ui_vocabularyopenprogressdialog.h>

#include <QtWidgets/QDialog>
#include "vocabulary.h"

class VocabularyOpenProgressDialog : public QDialog
{
	Q_OBJECT

	public:
		         VocabularyOpenProgressDialog(const Vocabulary *vocabulary, QWidget *parent = NULL, Qt::WindowFlags flags = 0);
		virtual ~VocabularyOpenProgressDialog();

	private:
		Ui::VocabularyOpenProgressDialog _ui;

	private slots:
		void on_vocabulary_setOpenProgressValue(quint8 value)        const;
		void on_vocabulary_setOpenProgressMax  (quint8 max)          const;
		void on_vocabulary_setVocabularyName   (const QString &name) const;
}; // VocabularyOpenProgressDialog

#endif // VOCABULARYOPENPROGRESSDIALOG_H