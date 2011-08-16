#include "vocabularymanagerdialog/wordsimportdialog.h"

#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>

WordsImportDialog::~WordsImportDialog()
{
    _iiPlugin->Close();
} // ~WordsImportDialog

void WordsImportDialog::accept()
{
    ImportData(TargetVocabulary);
    QDialog::accept();
} // accept

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

    _qdwiWordsImport.qpbOk->setEnabled(!_bImporting && qismCategorySelection->hasSelection());
    if (_bImporting) {
	    _qdwiWordsImport.qpbCancel->setText(tr("Stop"));
    } else {
        _qdwiWordsImport.qpbCancel->setText(tr("Cancel"));
    } // if else
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

const void WordsImportDialog::ImportData(const eTarget &pTarget)
{
    _bImporting = true;
	EnableControls();
    _bInterrupt = false;

	// setup progessbar
	_qdwiWordsImport.qpbProgress->setMaximum(_iiPlugin->GetRecordCount());

	// patterns
	QStringList qslPatterns;
	for (int iPattern = 0; iPattern < _vVocabulary->GetFieldCount(); iPattern++) {
		QModelIndex qmiIndex = _wifmFieldsModel.index(iPattern, WordsImportFieldsModel::ColumnEditor);
		const QLineEdit *qleEditor = qobject_cast<const QLineEdit *>(_qdwiWordsImport.qtvFields->indexWidget(qmiIndex));
		qslPatterns.append(qleEditor->text());
	} // for

	QStringList qslMarks = _iiPlugin->GetMarks();
	int iRecordCount = _iiPlugin->GetRecordCount();

	int iCategoryId;
	switch (pTarget) {
		case TargetPreview:
			_qdwiWordsImport.qtwPreview->setRowCount(iRecordCount);
			break;
		case TargetVocabulary:
			QModelIndex qmiCategory = _qdwiWordsImport.qtvCategories->currentIndex();
			iCategoryId = _vVocabulary->GetCategoryId(qmiCategory.row());
	} // switch

	int iSkipCount = 0;
	for (int iRecord = 0; iRecord < iRecordCount && !_bInterrupt; iRecord++) {
		// get mark data
		bool bSkip = false;
		QStringList qslMarkData;
		foreach (QString qsMark, qslMarks) {
			QString qsData = _iiPlugin->GetRecordData(iRecord, qsMark);
			if (_qdwiWordsImport.qcbSkipPartialRecords->isChecked() && qsData.isEmpty()) {
				bSkip = true;
				iSkipCount++;
				break;
			} // if
			qslMarkData.append(qsData);
		} // foreach
		if (bSkip) {
			continue;
		} // if

        // get data
        QStringList qslData;
		for (int iColumn = 0; iColumn < _vVocabulary->GetFieldCount(); iColumn++) {
			QString qsText = qslPatterns.at(iColumn);

			for (int iMark = 0; iMark < qslMarks.size(); iMark++) {
				qsText.replace(qslMarks.at(iMark), qslMarkData.at(iMark));
			} // for
            qslData.append(qsText);
        } // for

        // insert data into target
        for (int iColumn = 0; iColumn < _vVocabulary->GetFieldCount(); iColumn++) {
			switch (pTarget) {
				case TargetPreview:
					{
						QTableWidgetItem *qtwiTableItem = new QTableWidgetItem(qslData.at(iColumn));
						qtwiTableItem->setFlags(qtwiTableItem->flags() ^ Qt::ItemIsEditable);
						_qdwiWordsImport.qtwPreview->setItem(iRecord - iSkipCount, iColumn, qtwiTableItem);
					}
					break;
				case TargetVocabulary:
					_vVocabulary->AddRecord(iCategoryId, qslData);
			} // switch
		} // for

		// progress
		_qdwiWordsImport.qpbProgress->setValue(iRecord);
		QCoreApplication::processEvents();
	} // for

	if (pTarget == TargetPreview && iSkipCount > 0) {
		_qdwiWordsImport.qtwPreview->setRowCount(iRecordCount - iSkipCount);
	} // if

	// setup progessbar
	_qdwiWordsImport.qpbProgress->setValue(0);

	EnableControls();
    _bImporting = false;
} // ImportData

const void WordsImportDialog::on_qpbPreviewRefresh_clicked(bool checked /* false */)
{
    ImportData(TargetPreview);
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

void WordsImportDialog::reject()
{
    if (_bImporting) {
        _bInterrupt = true;
    } else {
        QDialog::reject();
    } // if else
} // reject

WordsImportDialog::WordsImportDialog(const QString &pFile, Vocabulary *pVocabulary, ImpInterface *pPlugin, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _cmCategoriesModel(pVocabulary), _wifmFieldsModel(pVocabulary)
{
	_qsFile = pFile;
	_vVocabulary = pVocabulary;
	_iiPlugin = pPlugin;

    _bImporting = false;
    _bInterrupt = false;
} // WordsImportDialog