#include "vocabularymanagerdialog/wordsexportdialog.h"

#include "../../common/marklineedit.h"

const void WordsExportDialog::on_eiPlugin_ProgressExportSetMax(quint32 pMax) const
{
    _qdweWordsExport.qpbProgress->setMaximum(pMax);
} // on_eiPlugin_ProgressExportSetMax

const void WordsExportDialog::on_eiPlugin_ProgressExportSetValue(quint32 pValue) const
{
    _qdweWordsExport.qpbProgress->setValue(pValue);
} // on_eiPlugin_ProgressExportSetValue

const void WordsExportDialog::on_eiPlugin_VocabularyGetCategoryIds(ExpInterface::CategoryIdList *pCategoryIds) const
{
    const QItemSelectionModel *qismSelection = _qdweWordsExport.qtvCategories->selectionModel();
    QModelIndexList qmilSelected = qismSelection->selectedRows();

    foreach (QModelIndex qmiIndex, qmilSelected) {
        int iCategoryId = _vVocabulary->GetCategoryId(qmiIndex.row());
        pCategoryIds->append(iCategoryId);
    } // foreach
} // on_eiPlugin_VocabularyGetCategoryIds

const void WordsExportDialog::on_eiPlugin_VocabularyGetCategoryName(quint8 pCategoryId, QString *pName) const
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

const void WordsExportDialog::on_eiPlugin_VocabularyGetMarkText(quint32 pRecordId, const QString &pMark, QString *pText) const
{
    QStringList qslMarks;
    on_eiPlugin_VocabularyGetMarks(&qslMarks);
    int iMark = qslMarks.indexOf(pMark);
    int iFieldId = _vVocabulary->GetFieldId(iMark);

    *pText = _vVocabulary->GetDataText(pRecordId, iFieldId);
} // on_eiPlugin_VocabularyGetMarkText

const void WordsExportDialog::on_eiPlugin_VocabularyGetRecordCount(quint8 pCategoryId, quint32 *pCount) const
{
    *pCount = _vVocabulary->GetRecordCount(pCategoryId, _qdweWordsExport.qcbExportEnabledOnly->isChecked());
} // on_eiPlugin_VocabularyGetRecordCount

const void WordsExportDialog::on_eiPlugin_VocabularyGetRecordIds(quint8 pCategoryId, ExpInterface::RecordIdList *pRecordIds) const
{
    *pRecordIds = _vVocabulary->GetRecordIds(pCategoryId);
} // on_eiPlugin_VocabularyGetRecordCount

const void WordsExportDialog::on_qpbExport_clicked(bool checked /* false */)
{
    QModelIndex qmiIndex = _qdweWordsExport.qtvExpPlugins->currentIndex();
    ExpInterface *eiPlugin = _teplExpPlugins.at(qmiIndex.row()).expInterface;
    eiPlugin->beginExport();
} // on_qpbExport_clicked

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

		ExpInterface *eiPlugin = _teplExpPlugins.at(qmiIndex.row()).expInterface;

        // connections
		connect(eiPlugin, SIGNAL(ProgressExportSetMax(quint32)), SLOT(on_eiPlugin_ProgressExportSetMax(quint32)));
		connect(eiPlugin, SIGNAL(ProgressExportSetValue(quint32)), SLOT(on_eiPlugin_ProgressExportSetValue(quint32)));
        connect(eiPlugin, SIGNAL(VocabularyGetCategoryIds(ExpInterface::CategoryIdList *)), SLOT(on_eiPlugin_VocabularyGetCategoryIds(ExpInterface::CategoryIdList *)));
		connect(eiPlugin, SIGNAL(VocabularyGetCategoryName(quint8, QString *)), SLOT(on_eiPlugin_VocabularyGetCategoryName(quint8, QString *)));
        connect(eiPlugin, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_eiPlugin_VocabularyGetMarks(QStringList *)));
		connect(eiPlugin, SIGNAL(VocabularyGetMarkText(quint32, const QString &, QString *)), SLOT(on_eiPlugin_VocabularyGetMarkText(quint32, const QString &, QString *)));
		connect(eiPlugin, SIGNAL(VocabularyGetRecordCount(quint8, quint32 *)), SLOT(on_eiPlugin_VocabularyGetRecordCount(quint8, quint32 *)));
		connect(eiPlugin, SIGNAL(VocabularyGetRecordIds(quint8, ExpInterface::RecordIdList *)), SLOT(on_eiPlugin_VocabularyGetRecordIds(quint8, ExpInterface::RecordIdList *)));

		// setup page
		eiPlugin->setupUi(qwExpPlugin);
	} // if

    // set plugin page
    _qdweWordsExport.qswExpPlugins->setCurrentIndex(_qhExpPluginPage.value(qmiIndex.row()));

    _qdweWordsExport.qpbExport->setEnabled(true);
} // on_qtvExpPluginsSelectionModel_selectionChanged

WordsExportDialog::WordsExportDialog(const Vocabulary *pVocabulary, const Plugins::ExpPluginList &pExpPlugins, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _cmCategoriesModel(pVocabulary), _epmExpPluginsModel(&pExpPlugins), _wefmFieldsModel(pVocabulary)
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
		_qdweWordsExport.qtvFields->header()->setSectionResizeMode(iColumn, QHeaderView::Stretch);
	} // for

	_qdweWordsExport.qtvCategories->selectAll();
} // WordsExportDialog