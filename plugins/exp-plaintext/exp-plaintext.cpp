#include "exp-plaintext.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>

ExpPlaintext::~ExpPlaintext()
{
}

void ExpPlaintext::beginExport() const
{
    // get filename
    const QString fileName = QFileDialog::getSaveFileName(_widget, QString(), QString(), tr("plaintext (*.txt)"));
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
	_widget = new PlaintextExportWidget(parent);
	QBoxLayout *boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
	boxLayout->insertWidget(WIDGET_POSITION, _widget);

	connect(_widget, SIGNAL(progressExportSetMax(quintptr)),                               SLOT(on_widget_progressExportSetMax(quintptr)));
	connect(_widget, SIGNAL(progressExportSetValue(quintptr)),                              SLOT(on_widget_progressExportSetValue(quintptr)));
	connect(_widget, SIGNAL(vocabularyGetCategoryIds(ExpInterface::CategoryIdList *)),     SLOT(on_widget_vocabularyGetCategoryIds(ExpInterface::CategoryIdList *)));
	connect(_widget, SIGNAL(vocabularyGetCategoryName(quintptr, QString *)),                 SLOT(on_widget_vocabularyGetCategoryName(quintptr, QString *)));
	connect(_widget, SIGNAL(vocabularyGetMarks(QStringList *)),                            SLOT(on_widget_vocabularyGetMarks(QStringList *)));
	connect(_widget, SIGNAL(vocabularyGetMarkText(quintptr, const QString &, QString *)),   SLOT(on_widget_vocabularyGetMarkText(quintptr, const QString &, QString *)));
	connect(_widget, SIGNAL(vocabularyGetRecordCount(quintptr, quintptr *)),                  SLOT(on_widget_vocabularyGetRecordCount(quintptr, quintptr *)));
	connect(_widget, SIGNAL(vocabularyGetRecordIds(quintptr, ExpInterface::RecordIdList *)), SLOT(on_widget_vocabularyGetRecordIds(quintptr, ExpInterface::RecordIdList *)));
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