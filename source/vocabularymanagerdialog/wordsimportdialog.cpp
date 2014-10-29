#include "vocabularymanagerdialog/wordsimportdialog.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>

WordsImportDialog::~WordsImportDialog()
{
    _iiPlugin->close();
} // ~WordsImportDialog

void WordsImportDialog::accept()
{
    ImportData(TargetVocabulary);
    if (!_bInterrupt) {
        QDialog::accept();
    } // if
} // accept

const void WordsImportDialog::CreateFieldEditors()
{
	_qdwiWordsImport.qtvFields->setItemDelegateForColumn(WordsImportFieldsModel::ColumnEditor, &_wiedEditorDelegate);

    for (int iRow = 0; iRow < _wifmFieldsModel.rowCount(); iRow++) {
		bool bPersistentEditor = true;

		int iFieldId = _vVocabulary->fieldId(iRow);
		if (_vVocabulary->fieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
			VocabularyDatabase::FieldBuiltIn efbBuiltIn = _vVocabulary->fieldBuiltIn(iFieldId);
			if (efbBuiltIn == VocabularyDatabase::FieldBuiltInEnabled) {
				bPersistentEditor = false;
			} // if
		} // if

		if (bPersistentEditor) {
			QModelIndex qmiIndex = _wifmFieldsModel.index(iRow, WordsImportFieldsModel::ColumnEditor);
			_qdwiWordsImport.qtvFields->openPersistentEditor(qmiIndex);
		} // if
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
    if (!_iiPlugin->open(_qsFile)) {
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
	CreateFieldEditors();
    _qdwiWordsImport.qtvFields->header()->setSectionResizeMode(WordsImportFieldsModel::ColumnName, QHeaderView::ResizeToContents);
	_qdwiWordsImport.qtvFields->header()->setSectionResizeMode(WordsImportFieldsModel::ColumnLanguage, QHeaderView::ResizeToContents);
    _qdwiWordsImport.qtvFields->header()->setSectionResizeMode(WordsImportFieldsModel::ColumnEditor, QHeaderView::Stretch);
	// preview
	PreparePreviewColumns();

    // plugin UI
    new QBoxLayout(QBoxLayout::LeftToRight, _qdwiWordsImport.qgbSource);
    _iiPlugin->setupUI(_qdwiWordsImport.qgbSource);

	return QDialog::exec();
} // exec

const void WordsImportDialog::ImportData(const eTarget &pTarget)
{
    _bImporting = true;
	EnableControls();
    _bInterrupt = false;

	// setup progessbar
	_qdwiWordsImport.qpbProgress->setMaximum(_iiPlugin->recordCount());

	// patterns
	QStringList qslPatterns;
	for (int iPattern = 0; iPattern < _vVocabulary->fieldCount(); iPattern++) {
		QModelIndex qmiIndex = _wifmFieldsModel.index(iPattern, WordsImportFieldsModel::ColumnEditor);
		QString qsData = _wifmFieldsModel.data(qmiIndex, Qt::EditRole).toString();
		qslPatterns.append(qsData);
	} // for

	QStringList qslMarks = _iiPlugin->marks();
	int iRecordCount = _iiPlugin->recordCount();

	int iCategoryId;
	switch (pTarget) {
		case TargetPreview:
			_qdwiWordsImport.qtwPreview->setRowCount(iRecordCount);
			break;
		case TargetVocabulary:
			QModelIndex qmiCategory = _qdwiWordsImport.qtvCategories->currentIndex();
			iCategoryId = _vVocabulary->categoryId(qmiCategory.row());
	} // switch

	int iSkipCount = 0;
	for (int iRecord = 0; iRecord < iRecordCount && !_bInterrupt; iRecord++) {
		// get mark data
		bool bSkip = false;
		QStringList qslMarkData;
		foreach (QString qsMark, qslMarks) {
			QString qsData = _iiPlugin->recordData(iRecord, qsMark);
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
		for (int iColumn = 0; iColumn < _vVocabulary->fieldCount(); iColumn++) {
			QString qsText = qslPatterns.at(iColumn);

			for (int iMark = 0; iMark < qslMarks.size(); iMark++) {
				qsText.replace(qslMarks.at(iMark), qslMarkData.at(iMark));
			} // for
            qslData.append(qsText);
        } // for

        // insert data into target
        switch (pTarget) {
            case TargetPreview:
                for (int iColumn = 0; iColumn < _vVocabulary->fieldCount(); iColumn++) {
					QTableWidgetItem *qtwiTableItem;

					int iFieldId = _vVocabulary->fieldId(iColumn);
					switch (_vVocabulary->fieldType(iFieldId)) {
						case VocabularyDatabase::FieldTypeLineEdit:
							qtwiTableItem = new QTableWidgetItem(qslData.at(iColumn));
							break;
						case VocabularyDatabase::FieldTypeCheckBox:
							qtwiTableItem = new QTableWidgetItem();
							qtwiTableItem->setCheckState(static_cast<Qt::CheckState>(qslData.at(iColumn).toInt()));
							qtwiTableItem->setFlags(qtwiTableItem->flags() | Qt::ItemIsUserCheckable);
							break;
						case VocabularyDatabase::FieldTypeSpinBox:
							qtwiTableItem = new QTableWidgetItem(qslData.at(iColumn));
					} // switch

					qtwiTableItem->setFlags(qtwiTableItem->flags() ^ Qt::ItemIsEditable);
					_qdwiWordsImport.qtwPreview->setItem(iRecord - iSkipCount, iColumn, qtwiTableItem);
				} // for
				break;
			case TargetVocabulary:
				_vVocabulary->addRecord(iCategoryId, qslData);
		} // switch

		// progress
        if (iRecord % IMPORT_REFRESHINTERVAL == 0) {
		    _qdwiWordsImport.qpbProgress->setValue(iRecord);
		    QCoreApplication::processEvents();
        } // if
	} // for

	if (pTarget == TargetPreview && iSkipCount > 0) {
		_qdwiWordsImport.qtwPreview->setRowCount(iRecordCount - iSkipCount);
	} // if

	// setup progessbar
	_qdwiWordsImport.qpbProgress->setValue(0);
	_qdwiWordsImport.qpbProgress->setMaximum(1);

    _bImporting = false;
    EnableControls();
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
	foreach (int iFieldId, _vVocabulary->fieldIds()) {
		qslColumns.append(_vVocabulary->fieldName(iFieldId));
	} // foreach

	_qdwiWordsImport.qtwPreview->setColumnCount(qslColumns.size());
	_qdwiWordsImport.qtwPreview->setHorizontalHeaderLabels(qslColumns);
	for (int iColumn = 0; iColumn < _qdwiWordsImport.qtwPreview->horizontalHeader()->count(); iColumn++) {
		_qdwiWordsImport.qtwPreview->horizontalHeader()->setSectionResizeMode(iColumn, QHeaderView::Stretch);
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

WordsImportDialog::WordsImportDialog(const QString &pFile, Vocabulary *pVocabulary, ImpInterface *pPlugin, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _cmCategoriesModel(pVocabulary), _wiedEditorDelegate(pVocabulary), _wifmFieldsModel(pVocabulary)
{
	_qsFile = pFile;
	_vVocabulary = pVocabulary;
	_iiPlugin = pPlugin;

    _bImporting = false;
    _bInterrupt = false;
} // WordsImportDialog