#include "plaintextimportwidget.h"

const int PlaintextImportWidget::GetLineCount() const
{
	int iFileLines = 0;
	_pfFile->Seek(PlaintextFile::PFILE_BEGIN);
	while (!_pfFile->ReadLine().isNull()) {
		iFileLines++;
	} // while

	return iFileLines;
} // GetLineCount

const int PlaintextImportWidget::GetLinesPerRecord() const
{
	return _qwpiPlaintextImport.qsbLinesPerRecord->value();
} // GetLinesPerRecord

const QString PlaintextImportWidget::GetRegExp() const
{
	return _qwpiPlaintextImport.qleRegExp->text();
} // GetRegExp

const void PlaintextImportWidget::on_qsbLinesPerRecord_valueChanged(int i) const
{
	RefreshPreview();
} // on_qsbLinesPerRecord_valueChanged

const void PlaintextImportWidget::on_qtvCodecsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	QModelIndex qmiIndex = _qwpiPlaintextImport.qtvCodecs->currentIndex();
	QString qsCodec = _cmCodecsModel.data(qmiIndex).toString();
	_pfFile->SetCodecName(qsCodec);

	_qwpiPlaintextImport.qsbLinesPerRecord->setMaximum(GetLineCount());
	RefreshPreview();
} // on_qtvCodecsSelectionModel_selectionChanged

PlaintextImportWidget::PlaintextImportWidget(PlaintextFile *pFile, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(pParent, pFlags)
{
	_pfFile = pFile;

    _qwpiPlaintextImport.setupUi(this);

    _qwpiPlaintextImport.qtvCodecs->setModel(&_cmCodecsModel);
	connect(_qwpiPlaintextImport.qtvCodecs->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvCodecsSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));

	PreselectCodec();

	_qwpiPlaintextImport.qsbLinesPerRecord->setMaximum(GetLineCount());
} // PlaintextImportWidget

const void PlaintextImportWidget::PreselectCodec() const
{
	QString qsFileCodec = _pfFile->GetCodecName();
	for (int iRow = 0; iRow < _cmCodecsModel.rowCount(); iRow++) {
		QModelIndex qmiIndex = _cmCodecsModel.index(iRow, CodecsModel::ColumnCodec);
		QString qsCodec = _cmCodecsModel.data(qmiIndex).toString();

		if (qsFileCodec == qsCodec) {
			_qwpiPlaintextImport.qtvCodecs->blockSignals(true);
			_qwpiPlaintextImport.qtvCodecs->setCurrentIndex(qmiIndex);
			_qwpiPlaintextImport.qtvCodecs->blockSignals(false);
			RefreshPreview();
			return;
		} // if
	} // for
} // PreselectCodec

const void PlaintextImportWidget::RefreshPreview() const
{
	_pfFile->Seek(PlaintextFile::PFILE_BEGIN);
	QString qsText;
	for (int iI = 0; iI < GetLinesPerRecord(); iI++) {
		if (!qsText.isEmpty()) {
			qsText += "\n";
		} // if
		qsText += _pfFile->ReadLine();
	} // for

	_qwpiPlaintextImport.qptePreview->setPlainText(qsText);
} // RefreshPreview