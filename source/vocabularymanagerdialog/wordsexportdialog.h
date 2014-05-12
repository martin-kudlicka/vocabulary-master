#ifndef WORDSEXPORTDIALOG_H
#define WORDSEXPORTDIALOG_H

#include <ui_wordsexportdialog.h>

#include "common/categoriesmodel.h"
#include "vocabularymanagerdialog/wordsexportdialog/wordsexportfieldsmodel.h"
#include "vocabularymanagerdialog/wordsexportdialog/exppluginsmodel.h"
#include "../../common/marklineeditpersistentdelegate.h"

class WordsExportDialog : public QDialog
{
	Q_OBJECT

	public:
		WordsExportDialog(const Vocabulary *pVocabulary, const Plugins::tExpPluginList &pExpPlugins, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		CategoriesModel _cmCategoriesModel;
		ExpPluginsModel _epmExpPluginsModel;
        MarkLineEditPersistentDelegate _mlepdMarkDelegate;
		Plugins::tExpPluginList _teplExpPlugins;
		Ui::qdWordsExport _qdweWordsExport;
		QHash<int, int> _qhExpPluginPage;
		const Vocabulary *_vVocabulary;
		WordsExportFieldsModel _wefmFieldsModel;

	private slots:
        const void on_eiPlugin_ProgressExportSetMax(const int &pMax) const;
        const void on_eiPlugin_ProgressExportSetValue(const int &pValue) const;
        const void on_eiPlugin_VocabularyGetCategoryIds(ExpInterface::CategoryIdList *pCategoryIds) const;
        const void on_eiPlugin_VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const;
        const void on_eiPlugin_VocabularyGetMarks(QStringList *pMarks) const;
        const void on_eiPlugin_VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const;
        const void on_eiPlugin_VocabularyGetRecordCount(const int &pCategoryId, int *pCount) const;
        const void on_eiPlugin_VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::RecordIdList *pRecordIds) const;
        const void on_qpbExport_clicked(bool checked = false);
		const void on_qtvExpPluginsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
}; // WordsExportDialog

#endif // WORDSEXPORTDIALOG_H