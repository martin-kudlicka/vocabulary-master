#ifndef WORDSIMPORTDIALOG_H
#define WORDSIMPORTDIALOG_H

#include <ui_wordsimportdialog.h>

#include "vocabulary.h"
#include "vocabularymanagerdialog/wordsimportdialog/importfilter.h"

class WordsImportDialog : public QDialog
{
	Q_ENUMS(eFileType)

	public:
		~WordsImportDialog();
		WordsImportDialog(const QString &pFile, const QString &pFilter, const Vocabulary *pVocabulary, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

		int exec();
		static const QString GetFilter();

	private:
		enum eFileType {
			FileTypeAnki
		}; // eFileType

		ImportFilter *_ifImportFilter;
		QString _qsFile;
		QString _qsFilter;
		Ui::qdWordsImport _qdwiWordsImport;
		const Vocabulary *_vVocabulary;
}; // WordsImportDialog

#endif // WORDSIMPORTDIALOG_H