#include "pch.h"
#include "exp-plaintext.h"

#include "plaintextexportwidget.h"

void ExpPlaintext::beginExport() const
{
  // get filename
  const auto fileName = QFileDialog::getSaveFileName(_widget, QString(), QString(), tr("plaintext (*.txt)"));
  if (fileName.isEmpty())
  {
    return;
  }

  // open file
  QFile file(fileName);
  file.open(QIODevice::WriteOnly | QIODevice::Text);

  // text stream
  QTextStream textStream(&file);
  textStream.setCodec(_widget->codec().toLocal8Bit());

  // process export
  _widget->setUpdatesEnabled(false);
  _widget->refresh();

  // save result to file
  textStream << _widget->text();
}

QString ExpPlaintext::pluginName() const
{
  return tr("text file (txt)");
}

void ExpPlaintext::setupUi(QWidget *parent)
{
  _widget        = new PlaintextExportWidget(parent);
  auto boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
  boxLayout->insertWidget(WIDGET_POSITION, _widget);

  connect(_widget, &PlaintextExportWidget::progressExportSetMax,      this, &ExpPlaintext::on_widget_progressExportSetMax);
  connect(_widget, &PlaintextExportWidget::progressExportSetValue,    this, &ExpPlaintext::on_widget_progressExportSetValue);
  connect(_widget, &PlaintextExportWidget::vocabularyGetCategoryIds,  this, &ExpPlaintext::on_widget_vocabularyGetCategoryIds);
  connect(_widget, &PlaintextExportWidget::vocabularyGetCategoryName, this, &ExpPlaintext::on_widget_vocabularyGetCategoryName);
  connect(_widget, &PlaintextExportWidget::vocabularyGetMarks,        this, &ExpPlaintext::on_widget_vocabularyGetMarks);
  connect(_widget, &PlaintextExportWidget::vocabularyGetMarkText,     this, &ExpPlaintext::on_widget_vocabularyGetMarkText);
  connect(_widget, &PlaintextExportWidget::vocabularyGetRecordCount,  this, &ExpPlaintext::on_widget_vocabularyGetRecordCount);
  connect(_widget, &PlaintextExportWidget::vocabularyGetRecordIds,    this, &ExpPlaintext::on_widget_vocabularyGetRecordIds);
}

void ExpPlaintext::on_widget_progressExportSetMax(quintptr max) const
{
  emit progressExportSetMax(max);
}

void ExpPlaintext::on_widget_progressExportSetValue(quintptr value) const
{
  emit progressExportSetValue(value);
}

void ExpPlaintext::on_widget_vocabularyGetCategoryIds(ExpInterface::CategoryIdList *categoryIds) const
{
  emit vocabularyGetCategoryIds(categoryIds);
}

void ExpPlaintext::on_widget_vocabularyGetCategoryName(quintptr categoryId, QString *name) const
{
  emit vocabularyGetCategoryName(categoryId, name);
}

void ExpPlaintext::on_widget_vocabularyGetMarks(QStringList *marks) const
{
  emit vocabularyGetMarks(marks);
}

void ExpPlaintext::on_widget_vocabularyGetMarkText(quintptr recordId, const QString &mark, QString *text) const
{
  emit vocabularyGetMarkText(recordId, mark, text);
}

void ExpPlaintext::on_widget_vocabularyGetRecordCount(quintptr categoryId, quintptr *count) const
{
  emit vocabularyGetRecordCount(categoryId, count);
}

void ExpPlaintext::on_widget_vocabularyGetRecordIds(quintptr categoryId, ExpInterface::RecordIdList *recordIds) const
{
  emit vocabularyGetRecordIds(categoryId, recordIds);
}