#include "vocabularymanagerdialog/wordsexportdialog.h"

const void WordsExportDialog::on_qtvExpPluginsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	QModelIndex qmiIndex = _qdweWordsExport.qtvExpPlugins->currentIndex();
	if (_qhExpPluginPage.contains(qmiIndex.row())) {
		// set plugin page
		_qdweWordsExport.qswExpPlugins->setCurrentIndex(_qhExpPluginPage.value(qmiIndex.row()));
	} else {
		// create page for new new plugin
		QWidget *qwExpPlugin = new QWidget(_qdweWordsExport.qswExpPlugins);
		QBoxLayout *qblLayout = new QBoxLayout(QBoxLayout::LeftToRight, qwExpPlugin);
		qblLayout->setContentsMargins(QMargins());

		// insert page to table
		int iPage = _qdweWordsExport.qswExpPlugins->addWidget(qwExpPlugin);
		_qhExpPluginPage.insert(qmiIndex.row(), iPage);

		// setup page
		ExpInterface *eiPlugin = _teplExpPlugins.at(qmiIndex.row());
		eiPlugin->SetupUI(qwExpPlugin);
	} // if else
} // on_qtvExpPluginsSelectionModel_selectionChanged

WordsExportDialog::WordsExportDialog(const Vocabulary *pVocabulary, const Plugins::tExpPluginList &pExpPlugins, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _cmCategoriesModel(pVocabulary), _epmExpPluginsModel(&pExpPlugins), _wefmFieldsModel(pVocabulary)
{
	_vVocabulary = pVocabulary;
	_teplExpPlugins = pExpPlugins;

	_qdweWordsExport.setupUi(this);

	// export plugins
	_qdweWordsExport.qtvExpPlugins->setModel(&_epmExpPluginsModel);
	connect(_qdweWordsExport.qtvExpPlugins->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvExpPluginsSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));

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