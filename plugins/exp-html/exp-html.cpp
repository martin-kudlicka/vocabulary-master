#include "exp-html.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>

ExpHtml::~ExpHtml()
{
} // ~ExpHtml

void ExpHtml::beginExport() const
{
    // get filename
    const QString fileName = QFileDialog::getSaveFileName(_widget, QString(), QString(), tr("html (*.html)"));
    if (fileName.isEmpty())
	{
        return;
    } // if

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
} // beginExport

QString ExpHtml::pluginName() const
{
	return tr("HTML page (html)");
} // pluginName

void ExpHtml::setupUi(QWidget *parent)
{
	_widget               = new HtmlExportWidget(parent);
	QBoxLayout *boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
	boxLayout->insertWidget(WIDGET_POSITION, _widget);

	connect(_widget, SIGNAL(progressExportSetMax(quint32)),                                SLOT(on_widget_progressExportSetMax(quint32)));
	connect(_widget, SIGNAL(progressExportSetValue(quint32)),                              SLOT(on_widget_progressExportSetValue(quint32)));
	connect(_widget, SIGNAL(vocabularyGetCategoryIds(ExpInterface::CategoryIdList *)),     SLOT(on_widget_vocabularyGetCategoryIds(ExpInterface::CategoryIdList *)));
	connect(_widget, SIGNAL(vocabularyGetCategoryName(quint8, QString *)),                 SLOT(on_widget_vocabularyGetCategoryName(quint8, QString *)));
	connect(_widget, SIGNAL(vocabularyGetMarks(QStringList *)),                            SLOT(on_widget_vocabularyGetMarks(QStringList *)));
	connect(_widget, SIGNAL(vocabularyGetMarkText(quint32, const QString &, QString *)),   SLOT(on_widget_vocabularyGetMarkText(quint32, const QString &, QString *)));
	connect(_widget, SIGNAL(vocabularyGetRecordCount(quint8, quint32 *)),                  SLOT(on_widget_vocabularyGetRecordCount(quint8, quint32 *)));
	connect(_widget, SIGNAL(vocabularyGetRecordIds(quint8, ExpInterface::RecordIdList *)), SLOT(on_widget_vocabularyGetRecordIds(quint8, ExpInterface::RecordIdList *)));
} // setupUi

void ExpHtml::on_widget_progressExportSetMax(quint32 max) const
{
    emit progressExportSetMax(max);
} // on_widget_progressExportSetMax

void ExpHtml::on_widget_progressExportSetValue(quint32 value) const
{
    emit progressExportSetValue(value);
} // on_widget_progressExportSetValue

void ExpHtml::on_widget_vocabularyGetCategoryIds(ExpInterface::CategoryIdList *categoryIds) const
{
    emit vocabularyGetCategoryIds(categoryIds);
} // on_widget_vocabularyGetCategoryIds

void ExpHtml::on_widget_vocabularyGetCategoryName(quint8 categoryId, QString *name) const
{
    emit vocabularyGetCategoryName(categoryId, name);
} // on_widget_vocabularyGetCategoryName

void ExpHtml::on_widget_vocabularyGetMarks(QStringList *marks) const
{
    emit vocabularyGetMarks(marks);
} // on_widget_vocabularyGetMarks

void ExpHtml::on_widget_vocabularyGetMarkText(quint32 recordId, const QString &mark, QString *text) const
{
    emit vocabularyGetMarkText(recordId, mark, text);
} // on_widget_vocabularyGetMarkText

void ExpHtml::on_widget_vocabularyGetRecordCount(quint8 categoryId, quint32 *count) const
{
    emit vocabularyGetRecordCount(categoryId, count);
} // on_widget_vocabularyGetRecordCount

void ExpHtml::on_widget_vocabularyGetRecordIds(quint8 categoryId, ExpInterface::RecordIdList *recordIds) const
{
    emit vocabularyGetRecordIds(categoryId, recordIds);
} // on_widget_vocabularyGetRecordCount