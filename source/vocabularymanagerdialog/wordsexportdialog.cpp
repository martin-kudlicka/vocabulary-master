#include "vocabularymanagerdialog/wordsexportdialog.h"

WordsExportDialog::WordsExportDialog(const Vocabulary *pVocabulary, const Plugins::tExpPluginList &pExpPlugins, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _cmCategoriesModel(pVocabulary), _epmExpPluginsModel(&pExpPlugins), _wefmFieldsModel(pVocabulary)
{
	_vVocabulary = pVocabulary;
	_teplExpPlugins = pExpPlugins;

	_qdweWordsExport.setupUi(this);

	// export plugins
	_qdweWordsExport.qtvExpPlugins->setModel(&_epmExpPluginsModel);

	// categories
	_qdweWordsExport.qtvCategories->setModel(&_cmCategoriesModel);
	// fields
	_qdweWordsExport.qtvFields->setModel(&_wefmFieldsModel);
	for (int iColumn = 0; iColumn < _qdweWordsExport.qtvFields->header()->count(); iColumn++) {
		_qdweWordsExport.qtvFields->header()->setResizeMode(iColumn, QHeaderView::Stretch);
	} // for

	_qdweWordsExport.qtvCategories->selectAll();
	_qdweWordsExport.qtvFields->selectAll();
} // WordsExportDialog