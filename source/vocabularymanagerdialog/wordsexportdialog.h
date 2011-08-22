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

		const void PrepareMarks() const;

	private slots:
        const void on_eiPlugin_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const;
        const void on_eiPlugin_VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const;
        const void on_eiPlugin_VocabularyGetMarks(QStringList *pMarks) const;
        const void on_eiPlugin_VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const;
        const void on_eiPlugin_VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const;
		const void on_qtvExpPluginsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
}; // WordsExportDialog

#endif // WORDSEXPORTDIALOG_H