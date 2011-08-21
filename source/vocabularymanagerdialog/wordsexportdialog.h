#ifndef WORDSEXPORTDIALOG_H
#define WORDSEXPORTDIALOG_H

#include <ui_wordsexportdialog.h>

#include "vocabulary.h"
#include "plugins.h"
#include "common/categoriesmodel.h"
#include "vocabularymanagerdialog/wordsexportdialog/wordsexportfieldsmodel.h"

class WordsExportDialog : public QDialog
{
	Q_OBJECT

	public:
		WordsExportDialog(const Vocabulary *pVocabulary, const Plugins::tExpPluginList &pExpPlugins, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		CategoriesModel _cmCategoriesModel;
		Plugins::tExpPluginList _teplExpPlugins;
		Ui::qdWordsExport _qdweWordsExport;
		const Vocabulary *_vVocabulary;
		WordsExportFieldsModel _wefmFieldsModel;
}; // WordsExportDialog

#endif // WORDSEXPORTDIALOG_H