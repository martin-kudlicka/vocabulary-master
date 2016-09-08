#include "exp-html.h"

#include "htmlexportwidget.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>

ExpHtml::~ExpHtml()
{
}

void ExpHtml::beginExport() const
{
  // get filename
  const auto fileName = QFileDialog::getSaveFileName(_widget, QString(), QString(), tr("html (*.html)"));
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

QString ExpHtml::pluginName() const
{
  return tr("HTML page (html)");
}

void ExpHtml::setupUi(QWidget *parent)
{
  _widget = new HtmlExportWidget(parent);
  auto *boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
  boxLayout->insertWidget(WIDGET_POSITION, _widget);

  connect(_widget, &HtmlExportWidget::progressExportSetMax,      this, &ExpHtml::on_widget_progressExportSetMax);
  connect(_widget, &HtmlExportWidget::progressExportSetValue,    this, &ExpHtml::on_widget_progressExportSetValue);
  connect(_widget, &HtmlExportWidget::vocabularyGetCategoryIds,  this, &ExpHtml::on_widget_vocabularyGetCategoryIds);
  connect(_widget, &HtmlExportWidget::vocabularyGetCategoryName, this, &ExpHtml::on_widget_vocabularyGetCategoryName);
  connect(_widget, &HtmlExportWidget::vocabularyGetMarks,        this, &ExpHtml::on_widget_vocabularyGetMarks);
  connect(_widget, &HtmlExportWidget::vocabularyGetMarkText,     this, &ExpHtml::on_widget_vocabularyGetMarkText);
  connect(_widget, &HtmlExportWidget::vocabularyGetRecordCount,  this, &ExpHtml::on_widget_vocabularyGetRecordCount);
  connect(_widget, &HtmlExportWidget::vocabularyGetRecordIds,    this, &ExpHtml::on_widget_vocabularyGetRecordIds);
}

void ExpHtml::on_widget_progressExportSetMax(quintptr max) const
{
  emit progressExportSetMax(max);
}

void ExpHtml::on_widget_progressExportSetValue(quintptr value) const
{
  emit progressExportSetValue(value);
}

void ExpHtml::on_widget_vocabularyGetCategoryIds(ExpInterface::CategoryIdList *categoryIds) const
{
  emit vocabularyGetCategoryIds(categoryIds);
}

void ExpHtml::on_widget_vocabularyGetCategoryName(quintptr categoryId, QString *name) const
{
  emit vocabularyGetCategoryName(categoryId, name);
}

void ExpHtml::on_widget_vocabularyGetMarks(QStringList *marks) const
{
  emit vocabularyGetMarks(marks);
}

void ExpHtml::on_widget_vocabularyGetMarkText(quintptr recordId, const QString &mark, QString *text) const
{
  emit vocabularyGetMarkText(recordId, mark, text);
}

void ExpHtml::on_widget_vocabularyGetRecordCount(quintptr categoryId, quintptr *count) const
{
  emit vocabularyGetRecordCount(categoryId, count);
}

void ExpHtml::on_widget_vocabularyGetRecordIds(quintptr categoryId, ExpInterface::RecordIdList *recordIds) const
{
  emit vocabularyGetRecordIds(categoryId, recordIds);
}