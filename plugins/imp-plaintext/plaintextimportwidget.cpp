#include "pch.h"
#include "plaintextimportwidget.h"

#include "plaintextimportwidget/plaintextfile.h"

PlaintextImportWidget::PlaintextImportWidget(PlaintextFile *file, QWidget *parent) : QWidget(parent), _file(file)
{
  _ui.setupUi(this);

  _ui.codecs->setModel(&_codecsModel);
  connect(_ui.codecs->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PlaintextImportWidget::on_codecs_selectionModel_selectionChanged);

  preselectCodec();

  _ui.linesPerRecord->setMaximum(lineCount());
}

quintptr PlaintextImportWidget::lineCount() const
{
  auto fileLines = 0;
  _file->seek(PlaintextFile::PFILE_BEGIN);
  while (!_file->readLine().isNull())
  {
    ++fileLines;
  }

  return fileLines;
}

quintptr PlaintextImportWidget::linesPerRecord() const
{
  return _ui.linesPerRecord->value();
}

QString PlaintextImportWidget::regExp() const
{
  return _ui.regExp->text();
}

void PlaintextImportWidget::preselectCodec() const
{
  const auto fileCodec = _file->codecName();
  for (auto codecIndex = 0; codecIndex < _codecsModel.rowCount(); ++codecIndex)
  {
    const auto modelIndex = _codecsModel.index(codecIndex, static_cast<int>(CodecsModel::Column::Codec));
    const auto codec      = _codecsModel.data(modelIndex).toString();

    if (fileCodec == codec)
    {
      _ui.codecs->blockSignals(true);
      _ui.codecs->setCurrentIndex(modelIndex);
      _ui.codecs->blockSignals(false);
      refreshPreview();
      return;
    }
  }
}

void PlaintextImportWidget::refreshPreview() const
{
  _file->seek(PlaintextFile::PFILE_BEGIN);
  QString text;
  for (decltype(linesPerRecord()) lineIndex = 0; lineIndex < linesPerRecord(); ++lineIndex)
  {
    if (!text.isEmpty())
    {
      text += "\n";
    }
    text += _file->readLine();
  }

  _ui.preview->setPlainText(text);
}

void PlaintextImportWidget::on_codecs_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  Q_UNUSED(selected);
  Q_UNUSED(deselected);

  const auto modelIndex = _ui.codecs->currentIndex();
  const auto codec      = _codecsModel.data(modelIndex).toString();
  _file->setCodecName(codec);

  _ui.linesPerRecord->setMaximum(lineCount());
  refreshPreview();
}

void PlaintextImportWidget::on_linesPerRecord_valueChanged(int i) const
{
  Q_UNUSED(i);

  refreshPreview();
}