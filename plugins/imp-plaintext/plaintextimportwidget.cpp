#include "plaintextimportwidget.h"

PlaintextImportWidget::PlaintextImportWidget(PlaintextFile *file, QWidget *parent /* nullptr */, Qt::WindowFlags pFlags /* 0 */) : QWidget(parent, pFlags), _file(file)
{
	_ui.setupUi(this);

	_ui.codecs->setModel(&_codecsModel);
	connect(_ui.codecs->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_codecs_selectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));

	preselectCodec();

	_ui.linesPerRecord->setMaximum(lineCount());
} // PlaintextImportWidget

PlaintextImportWidget::~PlaintextImportWidget()
{
} // ~PlaintextImportWidget

const quint16 PlaintextImportWidget::lineCount() const
{
	quint16 fileLines = 0;
	_file->seek(PlaintextFile::PFILE_BEGIN);
	while (!_file->readLine().isNull())
	{
		fileLines++;
	} // while

	return fileLines;
} // lineCount

const quint16 PlaintextImportWidget::linesPerRecord() const
{
	return _ui.linesPerRecord->value();
} // linesPerRecord

const QString PlaintextImportWidget::regExp() const
{
	return _ui.regExp->text();
} // regExp

const void PlaintextImportWidget::preselectCodec() const
{
	const QString fileCodec = _file->codecName();
	for (quint8 codecIndex = 0; codecIndex < _codecsModel.rowCount(); codecIndex++)
	{
		const QModelIndex modelIndex = _codecsModel.index(codecIndex, CodecsModel::ColumnCodec);
		const QString codec          = _codecsModel.data(modelIndex).toString();

		if (fileCodec == codec)
		{
			_ui.codecs->blockSignals(true);
			_ui.codecs->setCurrentIndex(modelIndex);
			_ui.codecs->blockSignals(false);
			refreshPreview();
			return;
		} // if
	} // for
} // preselectCodec

const void PlaintextImportWidget::refreshPreview() const
{
	_file->seek(PlaintextFile::PFILE_BEGIN);
	QString text;
	for (quint16 lineIndex = 0; lineIndex < linesPerRecord(); lineIndex++)
	{
		if (!text.isEmpty())
		{
			text += "\n";
		} // if
		text += _file->readLine();
	} // for

	_ui.preview->setPlainText(text);
} // refreshPreview

const void PlaintextImportWidget::on_codecs_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	const QModelIndex modelIndex = _ui.codecs->currentIndex();
	const QString codec          = _codecsModel.data(modelIndex).toString();
	_file->setCodecName(codec);

	_ui.linesPerRecord->setMaximum(lineCount());
	refreshPreview();
} // on_codecs_selectionModel_selectionChanged

const void PlaintextImportWidget::on_linesPerRecord_valueChanged(int i) const
{
	refreshPreview();
} // on_linesPerRecord_valueChanged