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

const void WordsImportDialog::on_qpbPreview_clicked(bool checked /* false */) const
{
	QStringList qslMarks = _iiPlugin->GetMarks();
	int iRecordCount = _iiPlugin->GetRecordCount();

	for (int iRecord = 0; iRecord < iRecordCount; iRecord++) {
		// get mark data
		foreach (QString qsMark, qslMarks) {
			QString qsData = _iiPlugin->GetRecordData(iRecord, qsMark);
		} // foreach

		for (int iColumn = 0; iColumn < _vVocabulary->GetFieldCount(); iColumn++) {
			// TODO
		} // for
	} // for
} // on_qpbPreview_clicked

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