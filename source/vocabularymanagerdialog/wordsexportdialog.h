#ifndef WORDSEXPORTDIALOG_H
#define WORDSEXPORTDIALOG_H

#include <ui_wordsexportdialog.h>

#include "common/categoriesmodel.h"
#include "vocabularymanagerdialog/wordsexportdialog/wordsexportfieldsmodel.h"
#include "vocabularymanagerdialog/wordsexportdialog/exppluginsmodel.h"

class WordsExportDialog : public QDialog
{
	Q_OBJECT

	public:
		WordsExportDialog(const Vocabulary *pVocabulary, const Plugins::tExpPluginList &pExpPlugins, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		CategoriesModel _cmCategoriesModel;
		ExpPluginsModel _epmExpPluginsModel;
		Plugins::tExpPluginList _teplExpPlugins;
		Ui::qdWordsExport _qdweWordsExport;
		QHash<int, int> _qhExpPluginPage;
		const Vocabulary *_vVocabulary;
		WordsExportFieldsModel _wefmFieldsModel;

	private slots:
		const void on_qtvExpPluginsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
}; // WordsExportDialog

#endif // WORDSEXPORTDIALOG_H