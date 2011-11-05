#include "exp-html.h"

#include <QtGui/QFileDialog>
#include <QtCore/QTextStream>

const void ExpHtml::BeginExport() const
{
    // get filename
    QString qsFile = QFileDialog::getSaveFileName(_hewWidget, QString(), QString(), tr("html (*.html)"));
    if (qsFile.isEmpty()) {
        return;
    } // if

    // open file
    QFile qfFile(qsFile);
    qfFile.open(QIODevice::WriteOnly | QIODevice::Text);

    // text stream
    QTextStream qtsTextStream(&qfFile);
    qtsTextStream.setCodec(_hewWidget->GetCodec().toLocal8Bit());

    // process export
    _hewWidget->setUpdatesEnabled(false);
    _hewWidget->Refresh();

    // save result to file
    qtsTextStream << _hewWidget->GetText();
} // BeginExport

const QString ExpHtml::GetPluginName() const
{
	return tr("HTML page (html)");
} // GetPluginName

const void ExpHtml::on_hewWidget_ProgressExportSetMax(const int &pMax) const
{
    emit ProgressExportSetMax(pMax);
} // on_hewWidget_ProgressExportSetMax

const void ExpHtml::on_hewWidget_ProgressExportSetValue(const int &pValue) const
{
    emit ProgressExportSetValue(pValue);
} // on_hewWidget_ProgressExportSetValue

const void ExpHtml::on_hewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const
{
    emit VocabularyGetCategoryIds(pCategoryIds);
} // on_hewWidget_VocabularyGetCategoryIds

const void ExpHtml::on_hewWidget_VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const
{
    emit VocabularyGetCategoryName(pCategoryId, pName);
} // on_hewWidget_VocabularyGetCategoryName

const void ExpHtml::on_hewWidget_VocabularyGetMarks(QStringList *pMarks) const
{
    emit VocabularyGetMarks(pMarks);
} // on_hewWidget_VocabularyGetMarks

const void ExpHtml::on_hewWidget_VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const
{
    emit VocabularyGetMarkText(pRecordId, pMark, pText);
} // on_hewWidget_VocabularyGetMarkText

const void ExpHtml::on_hewWidget_VocabularyGetRecordCount(const int &pCategoryId, int *pCount) const
{
    emit VocabularyGetRecordCount(pCategoryId, pCount);
} // on_hewWidget_VocabularyGetRecordCount

const void ExpHtml::on_hewWidget_VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const
{
    emit VocabularyGetRecordIds(pCategoryId, pRecordIds);
} // on_hewWidget_VocabularyGetRecordCount

const void ExpHtml::SetupUI(QWidget *pParent)
{
	_hewWidget = new HtmlExportWidget(pParent);
	QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
	pLayout->insertWidget(WIDGET_POSITION, _hewWidget);

    connect(_hewWidget, SIGNAL(ProgressExportSetMax(const int &)), SLOT(on_hewWidget_ProgressExportSetMax(const int &)));
    connect(_hewWidget, SIGNAL(ProgressExportSetValue(const int &)), SLOT(on_hewWidget_ProgressExportSetValue(const int &)));
    connect(_hewWidget, SIGNAL(VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)), SLOT(on_hewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)));
    connect(_hewWidget, SIGNAL(VocabularyGetCategoryName(const int &, QString *)), SLOT(on_hewWidget_VocabularyGetCategoryName(const int &, QString *)));
    connect(_hewWidget, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_hewWidget_VocabularyGetMarks(QStringList *)));
    connect(_hewWidget, SIGNAL(VocabularyGetMarkText(const int &, const QString &, QString *)), SLOT(on_hewWidget_VocabularyGetMarkText(const int &, const QString &, QString *)));
    connect(_hewWidget, SIGNAL(VocabularyGetRecordCount(const int &, int *)), SLOT(on_hewWidget_VocabularyGetRecordCount(const int &, int *)));
    connect(_hewWidget, SIGNAL(VocabularyGetRecordIds(const int &, ExpInterface::tRecordIdList *)), SLOT(on_hewWidget_VocabularyGetRecordIds(const int &, ExpInterface::tRecordIdList *)));
} // SetupUI

Q_EXPORT_PLUGIN2(exp-html, ExpHtml)