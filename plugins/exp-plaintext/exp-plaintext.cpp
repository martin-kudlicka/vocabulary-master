#include "exp-plaintext.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>

const void ExpPlaintext::beginExport() const
{
    // get filename
    const QString fileName = QFileDialog::getSaveFileName(_widget, QString(), QString(), tr("plaintext (*.txt)"));
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

const QString ExpPlaintext::pluginName() const
{
	return tr("text file (txt)");
} // pluginName

const void ExpPlaintext::setupUi(QWidget *parent)
{
	_widget = new PlaintextExportWidget(parent);
	QBoxLayout *boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
	boxLayout->insertWidget(WIDGET_POSITION, _widget);

	connect(_widget, SIGNAL(progressExportSetMax(const quint32 &)),                                SLOT(on_widget_progressExportSetMax(const quint32 &)));
	connect(_widget, SIGNAL(progressExportSetValue(const quint32 &)),                              SLOT(on_widget_progressExportSetValue(const quint32 &)));
	connect(_widget, SIGNAL(vocabularyGetCategoryIds(ExpInterface::CategoryIdList *)),             SLOT(on_widget_vocabularyGetCategoryIds(ExpInterface::CategoryIdList *)));
	connect(_widget, SIGNAL(vocabularyGetCategoryName(const quint8 &, QString *)),                 SLOT(on_widget_vocabularyGetCategoryName(const quint8 &, QString *)));
	connect(_widget, SIGNAL(vocabularyGetMarks(QStringList *)),                                    SLOT(on_widget_vocabularyGetMarks(QStringList *)));
	connect(_widget, SIGNAL(vocabularyGetMarkText(const quint32 &, const QString &, QString *)),   SLOT(on_widget_vocabularyGetMarkText(const quint32 &, const QString &, QString *)));
	connect(_widget, SIGNAL(vocabularyGetRecordCount(const quint8 &, quint32 *)),                  SLOT(on_widget_vocabularyGetRecordCount(const quint8 &, quint32 *)));
	connect(_widget, SIGNAL(vocabularyGetRecordIds(const quint8 &, ExpInterface::RecordIdList *)), SLOT(on_widget_vocabularyGetRecordIds(const quint8 &, ExpInterface::RecordIdList *)));
} // setupUi

const void ExpPlaintext::on_widget_progressExportSetMax(const quint32 &max) const
{
    emit progressExportSetMax(max);
} // on_widget_progressExportSetMax

const void ExpPlaintext::on_widget_progressExportSetValue(const quint32 &value) const
{
    emit progressExportSetValue(value);
} // on_widget_progressExportSetValue

const void ExpPlaintext::on_widget_vocabularyGetCategoryIds(ExpInterface::CategoryIdList *categoryIds) const
{
    emit vocabularyGetCategoryIds(categoryIds);
} // on_widget_vocabularyGetCategoryIds

const void ExpPlaintext::on_widget_vocabularyGetCategoryName(const quint8 &categoryId, QString *name) const
{
    emit vocabularyGetCategoryName(categoryId, name);
} // on_widget_vocabularyGetCategoryName

const void ExpPlaintext::on_widget_vocabularyGetMarks(QStringList *marks) const
{
    emit vocabularyGetMarks(marks);
} // on_widget_vocabularyGetMarks

const void ExpPlaintext::on_widget_vocabularyGetMarkText(const quint32 &recordId, const QString &mark, QString *text) const
{
    emit vocabularyGetMarkText(recordId, mark, text);
} // on_widget_vocabularyGetMarkText

const void ExpPlaintext::on_widget_vocabularyGetRecordCount(const quint8 &categoryId, quint32 *count) const
{
    emit vocabularyGetRecordCount(categoryId, count);
} // on_widget_vocabularyGetRecordCount

const void ExpPlaintext::on_widget_vocabularyGetRecordIds(const quint8 &categoryId, ExpInterface::RecordIdList *recordIds) const
{
    emit vocabularyGetRecordIds(categoryId, recordIds);
} // on_widget_vocabularyGetRecordIds