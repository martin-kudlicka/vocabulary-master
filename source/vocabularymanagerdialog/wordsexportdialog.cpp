#include "vocabularymanagerdialog/wordsexportdialog.h"

#include "../../common/marklineedit.h"

const void WordsExportDialog::on_eiPlugin_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const
{
    const QItemSelectionModel *qismSelection = _qdweWordsExport.qtvCategories->selectionModel();
    QModelIndexList qmilSelected = qismSelection->selectedRows();

    foreach (QModelIndex qmiIndex, qmilSelected) {
        int iCategoryId = _vVocabulary->GetCategoryId(qmiIndex.row());
        pCategoryIds->append(iCategoryId);
    } // foreach
} // on_eiPlugin_VocabularyGetCategoryIds

const void WordsExportDialog::on_eiPlugin_VocabularyGetMarks(QStringList *pMarks) const
{
    for (int iI = 0; iI < _wefmFieldsModel.rowCount(); iI++) {
        QModelIndex qmiEditorIndex = _wefmFieldsModel.index(iI, WordsExportFieldsModel::ColumnMark);
        const MarkLineEdit *mleEditor = qobject_cast<const MarkLineEdit *>(_qdweWordsExport.qtvFields->indexWidget(qmiEditorIndex));
        pMarks->append(mleEditor->text());
    } // for
} // on_eiPlugin_VocabularyGetMarks

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

        // connections
        connect(eiPlugin, SIGNAL(VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)), SLOT(on_eiPlugin_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)));
        connect(eiPlugin, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_eiPlugin_VocabularyGetMarks(QStringList *)));
	} // if else
} // on_qtvExpPluginsSelectionModel_selectionChanged

const void WordsExportDialog::PrepareMarks() const
{
	for (int iI = 0; iI < _wefmFieldsModel.rowCount(); iI++) {
		int iFieldId = _vVocabulary->GetFieldId(iI);
		QString qsName = _vVocabulary->GetFieldTemplateName(iFieldId);

		QModelIndex qmiEditorIndex = _wefmFieldsModel.index(iI, WordsExportFieldsModel::ColumnMark);
		MarkLineEdit *mleEditor = new MarkLineEdit(TEMPLATE_MARK.arg(qsName), _qdweWordsExport.qtvFields);
		_qdweWordsExport.qtvFields->setIndexWidget(qmiEditorIndex, mleEditor);
	} // for
} // PrepareMarks

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
	PrepareMarks();

	_qdweWordsExport.qtvCategories->selectAll();
} // WordsExportDialog