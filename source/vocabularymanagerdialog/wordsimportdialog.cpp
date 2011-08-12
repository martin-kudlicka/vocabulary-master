#include "vocabularymanagerdialog/wordsimportdialog.h"

#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>

const void WordsImportDialog::CreateFieldEditors() const
{
    for (int iRow = 0; iRow < _wifmFieldsModel.rowCount(); iRow++) {
        QModelIndex qmiIndex = _wifmFieldsModel.index(iRow, WordsImportFieldsModel::ColumnEditor);
        QLineEdit *qleEditor = new QLineEdit(_qdwiWordsImport.qtvFields);
        _qdwiWordsImport.qtvFields->setIndexWidget(qmiIndex, qleEditor);
    } // for
} // CreateFieldEditors

const void WordsImportDialog::EnableControls() const
{
    const QItemSelectionModel *qismCategorySelection = _qdwiWordsImport.qtvCategories->selectionModel();

    _qdwiWordsImport.qpbOk->setEnabled(qismCategorySelection->hasSelection());
} // EnableControls

int WordsImportDialog::exec()
{
    if (!_iiPlugin->Open(_qsFile)) {
        QMessageBox::critical(QApplication::activeWindow(), tr("Words import"), tr("Can't import data from selected file."));
        return QDialog::Rejected;
    } // if

    // vocabulary UI
    _qdwiWordsImport.setupUi(this);
    // categories
    _qdwiWordsImport.qtvCategories->setModel(&_cmCategoriesModel);
    connect(_qdwiWordsImport.qtvCategories->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvCategoriesSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
    // fields
    _qdwiWordsImport.qtvFields->setModel(&_wifmFieldsModel);
    _qdwiWordsImport.qtvFields->header()->setResizeMode(WordsImportFieldsModel::ColumnName, QHeaderView::ResizeToContents);
    _qdwiWordsImport.qtvFields->header()->setResizeMode(WordsImportFieldsModel::ColumnEditor, QHeaderView::Stretch);
    CreateFieldEditors();
	// preview
	PreparePreviewColumns();

    // plugin UI
    new QBoxLayout(QBoxLayout::LeftToRight, _qdwiWordsImport.qgbSource);
    _iiPlugin->SetupUI(_qdwiWordsImport.qgbSource);

	return QDialog::exec();
} // exec

const void WordsImportDialog::on_qpbPreviewRefresh_clicked(bool checked /* false */) const
{
    // patterns
    QStringList qslPatterns;
    for (int iPattern = 0; iPattern < _vVocabulary->GetFieldCount(); iPattern++) {
        QModelIndex qmiIndex = _wifmFieldsModel.index(iPattern, WordsImportFieldsModel::ColumnEditor);
        const QLineEdit *qleEditor = qobject_cast<const QLineEdit *>(_qdwiWordsImport.qtvFields->indexWidget(qmiIndex));
        qslPatterns.append(qleEditor->text());
    } // for

	QStringList qslMarks = _iiPlugin->GetMarks();
	int iRecordCount = _iiPlugin->GetRecordCount();
    _qdwiWordsImport.qtwPreview->setRowCount(iRecordCount);

	for (int iRecord = 0; iRecord < iRecordCount; iRecord++) {
		// get mark data
        QStringList qslMarkData;
		foreach (QString qsMark, qslMarks) {
			QString qsData = _iiPlugin->GetRecordData(iRecord, qsMark);
            qslMarkData.append(qsData);
		} // foreach

        // insert table columns
		for (int iColumn = 0; iColumn < _vVocabulary->GetFieldCount(); iColumn++) {
			QString qsText = qslPatterns.at(iColumn);

            for (int iMark = 0; iMark < qslMarks.size(); iMark++) {
                qsText.replace(qslMarks.at(iMark), qslMarkData.at(iMark));
            } // for

            QTableWidgetItem *qtwiTableItem = new QTableWidgetItem(qsText);
            _qdwiWordsImport.qtwPreview->setItem(iRecord, iColumn, qtwiTableItem);
		} // for
	} // for
} // on_qpbPreviewRefresh_clicked

const void WordsImportDialog::on_qtvCategoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
    EnableControls();
} // on_qtvCategoriesSelectionModel_selectionChanged

const void WordsImportDialog::PreparePreviewColumns() const
{
	QStringList qslColumns;
	foreach (int iFieldId, _vVocabulary->GetFieldIds()) {
		qslColumns.append(_vVocabulary->GetFieldName(iFieldId));
	} // foreach

	_qdwiWordsImport.qtwPreview->setColumnCount(qslColumns.size());
	_qdwiWordsImport.qtwPreview->setHorizontalHeaderLabels(qslColumns);
	for (int iColumn = 0; iColumn < _qdwiWordsImport.qtwPreview->horizontalHeader()->count(); iColumn++) {
		_qdwiWordsImport.qtwPreview->horizontalHeader()->setResizeMode(iColumn, QHeaderView::Stretch);
	} // for
} // PreparePreviewColumns

WordsImportDialog::WordsImportDialog(const QString &pFile, const Vocabulary *pVocabulary, ImpInterface *pPlugin, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _cmCategoriesModel(pVocabulary), _wifmFieldsModel(pVocabulary)
{
	_qsFile = pFile;
	_vVocabulary = pVocabulary;
	_iiPlugin = pPlugin;
} // WordsImportDialog