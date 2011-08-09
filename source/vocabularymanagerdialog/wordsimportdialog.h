#ifndef WORDSIMPORTDIALOG_H
#define WORDSIMPORTDIALOG_H

#include <ui_wordsimportdialog.h>

#include "vocabulary.h"
#include "../plugins/common/imp-interface.h"

class WordsImportDialog : public QDialog
{
	Q_ENUMS(eFileType)

	public:
		WordsImportDialog(const QString &pFile, const Vocabulary *pVocabulary, ImpInterface *pPlugin, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

		int exec();

	private:
		ImpInterface *_iiPlugin;
		QString _qsFile;
		Ui::qdWordsImport _qdwiWordsImport;
		const Vocabulary *_vVocabulary;
}; // WordsImportDialog

#endif // WORDSIMPORTDIALOG_H