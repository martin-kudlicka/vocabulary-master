#ifndef WORDSIMPORTDIALOG_H
#define WORDSIMPORTDIALOG_H

#include <ui_wordsimportdialog.h>

#include "vocabulary.h"
#include "plugins.h"

class WordsImportDialog : public QDialog
{
	Q_ENUMS(eFileType)

	public:
		WordsImportDialog(const QString &pFile, const QString &pFilter, const Vocabulary *pVocabulary, const Plugins *pPlugins, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

		int exec();

	private:
		ImpInterface *_iiPlugin;
		const Plugins *_pPlugins;
		QString _qsFile;
		QString _qsFilter;
		Ui::qdWordsImport _qdwiWordsImport;
		const Vocabulary *_vVocabulary;
}; // WordsImportDialog

#endif // WORDSIMPORTDIALOG_H