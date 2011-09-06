#include "vocabularymanagerdialog/wordsexportdialog.h"

#include "../../common/marklineedit.h"

void WordsExportDialog::accept()
{
    QModelIndex qmiIndex = _qdweWordsExport.qtvExpPlugins->currentIndex();
    ExpInterface *eiPlugin = _teplExpPlugins.at(qmiIndex.row()).eiInterface;
    if (eiPlugin->BeginExport()) {
        QDialog::accept();
    } // if
} // accept

const void WordsExportDialog::on_eiPlugin_ProgressExportSetMax(const int &pMax) const
{
    _qdweWordsExport.qpbProgress->setMaximum(pMax);
} // on_eiPlugin_ProgressExportSetMax

const void WordsExportDialog::on_eiPlugin_ProgressExportSetValue(const int &pValue) const
{
    _qdweWordsExport.qpbProgress->setValue(pValue);
} // on_eiPlugin_ProgressExportSetValue

const void WordsExportDialog::on_eiPlugin_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const
{
    const QItemSelectionModel *qismSelection = _qdweWordsExport.qtvCategories->selectionModel();
    QModelIndexList qmilSelected = qismSelection->selectedRows();

    foreach (QModelIndex qmiIndex, qmilSelected) {
        int iCategoryId = _vVocabulary->GetCategoryId(qmiIndex.row());
        pCategoryIds->append(iCategoryId);
    } // foreach
} // on_eiPlugin_VocabularyGetCategoryIds

const void WordsExportDialog::on_eiPlugin_VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const
{
    *pName = _vVocabulary->GetCategoryName(pCategoryId);
} // on_eiPlugin_VocabularyGetCategoryName

const void WordsExportDialog::on_eiPlugin_VocabularyGetMarks(QStringList *pMarks) const
{
    for (int iI = 0; iI < _wefmFieldsModel.rowCount(); iI++) {
        QModelIndex qmiEditorIndex = _wefmFieldsModel.index(iI, WordsExportFieldsModel::ColumnMark);
        const MarkLineEdit *mleEditor = qobject_cast<const MarkLineEdit *>(_qdweWordsExport.qtvFields->indexWidget(qmiEditorIndex));
        pMarks->append(mleEditor->text());
    } // for
} // on_eiPlugin_VocabularyGetMarks

const void WordsExportDialog::on_eiPlugin_VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const
{
    QStringList qslMarks;
    on_eiPlugin_VocabularyGetMarks(&qslMarks);
    int iMark = qslMarks.indexOf(pMark);
    int iFieldId = _vVocabulary->GetFieldId(iMark);

    *pText = _vVocabulary->GetDataText(pRecordId, iFieldId);
} // on_eiPlugin_VocabularyGetMarkText

const void WordsExportDialog::on_eiPlugin_VocabularyGetRecordCount(const int &pCategoryId, int *pCount) const
{
    *pCount = _vVocabulary->GetRecordCount(pCategoryId);
} // on_eiPlugin_VocabularyGetRecordCount

const void WordsExportDialog::on_eiPlugin_VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const
{
    *pRecordIds = _vVocabulary->GetRecordIds(pCategoryId);
} // on_eiPlugin_VocabularyGetRecordCount

const void WordsExportDialog::on_qtvExpPluginsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	QModelIndex qmiIndex = _qdweWordsExport.qtvExpPlugins->currentIndex();
	if (!_qhExpPluginPage.contains(qmiIndex.row())) {
		// create page for new new plugin
		QWidget *qwExpPlugin = new QWidget(_qdweWordsExport.qswExpPlugins);
		QBoxLayout *qblLayout = new QBoxLayout(QBoxLayout::LeftToRight, qwExpPlugin);
		qblLayout->setContentsMargins(QMargins());

		// insert page to table
		int iPage = _qdweWordsExport.qswExpPlugins->addWidget(qwExpPlugin);
		_qhExpPluginPage.insert(qmiIndex.row(), iPage);

		// setup page
		ExpInterface *eiPlugin = _teplExpPlugins.at(qmiIndex.row()).eiInterface;
		eiPlugin->SetupUI(qwExpPlugin);

        // connections
        connect(eiPlugin, SIGNAL(ProgressExportSetMax(const int &)), SLOT(on_eiPlugin_ProgressExportSetMax(const int &)));
        connect(eiPlugin, SIGNAL(ProgressExportSetValue(const int &)), SLOT(on_eiPlugin_ProgressExportSetValue(const int &)));
        connect(eiPlugin, SIGNAL(VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)), SLOT(on_eiPlugin_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)));
        connect(eiPlugin, SIGNAL(VocabularyGetCategoryName(const int &, QString *)), SLOT(on_eiPlugin_VocabularyGetCategoryName(const int &, QString *)));
        connect(eiPlugin, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_eiPlugin_VocabularyGetMarks(QStringList *)));
        connect(eiPlugin, SIGNAL(VocabularyGetMarkText(const int &, const QString &, QString *)), SLOT(on_eiPlugin_VocabularyGetMarkText(const int &, const QString &, QString *)));
        connect(eiPlugin, SIGNAL(VocabularyGetRecordCount(const int &, int *)), SLOT(on_eiPlugin_VocabularyGetRecordCount(const int &, int *)));
        connect(eiPlugin, SIGNAL(VocabularyGetRecordIds(const int &, ExpInterface::tRecordIdList *)), SLOT(on_eiPlugin_VocabularyGetRecordIds(const int &, ExpInterface::tRecordIdList *)));
	} // if

    // set plugin page
    _qdweWordsExport.qswExpPlugins->setCurrentIndex(_qhExpPluginPage.value(qmiIndex.row()));

    _qdweWordsExport.qpbOk->setEnabled(true);
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
    _qdweWordsExport.qtvFields->setItemDelegateForColumn(WordsExportFieldsModel::ColumnMark, &_mlepdMarkDelegate);
    for (int iRow = 0; iRow < _wefmFieldsModel.rowCount(); iRow++) {
        QModelIndex qmiIndex = _wefmFieldsModel.index(iRow, WordsExportFieldsModel::ColumnMark);
        _qdweWordsExport.qtvFields->openPersistentEditor(qmiIndex);
    } // for
	for (int iColumn = 0; iColumn < _qdweWordsExport.qtvFields->header()->count(); iColumn++) {
		_qdweWordsExport.qtvFields->header()->setResizeMode(iColumn, QHeaderView::Stretch);
	} // for

	_qdweWordsExport.qtvCategories->selectAll();
} // WordsExportDialog