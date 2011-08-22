#include "exp-plaintext.h"

#include "plaintextexportwidget.h"

const QString ExpPlaintext::GetPluginName() const
{
	return tr("plaintext (txt)");
} // GetPluginName

const void ExpPlaintext::on_pewWidget_ProgressExportSetMax(const int &pMax) const
{
    emit ProgressExportSetMax(pMax);
} // on_pewWidget_ProgressExportSetMax

const void ExpPlaintext::on_pewWidget_ProgressExportSetValue(const int &pValue) const
{
    emit ProgressExportSetValue(pValue);
} // on_pewWidget_ProgressExportSetValue

const void ExpPlaintext::on_pewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const
{
    emit VocabularyGetCategoryIds(pCategoryIds);
} // on_pewWidget_VocabularyGetCategoryIds

const void ExpPlaintext::on_pewWidget_VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const
{
    emit VocabularyGetCategoryName(pCategoryId, pName);
} // on_pewWidget_VocabularyGetCategoryName

const void ExpPlaintext::on_pewWidget_VocabularyGetMarks(QStringList *pMarks) const
{
    emit VocabularyGetMarks(pMarks);
} // on_pewWidget_VocabularyGetMarks

const void ExpPlaintext::on_pewWidget_VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const
{
    emit VocabularyGetMarkText(pRecordId, pMark, pText);
} // on_pewWidget_VocabularyGetMarkText

const void ExpPlaintext::on_pewWidget_VocabularyGetRecordCount(const int &pCategoryId, int *pCount) const
{
    emit VocabularyGetRecordCount(pCategoryId, pCount);
} // on_pewWidget_VocabularyGetRecordCount

const void ExpPlaintext::on_pewWidget_VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const
{
    emit VocabularyGetRecordIds(pCategoryId, pRecordIds);
} // on_pewWidget_VocabularyGetRecordCount

const void ExpPlaintext::SetupUI(QWidget *pParent)
{
	PlaintextExportWidget *pewWidget = new PlaintextExportWidget(pParent);
	QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
	pLayout->insertWidget(WIDGET_POSITION, pewWidget);

    connect(pewWidget, SIGNAL(ProgressExportSetMax(const int &)), SLOT(on_pewWidget_ProgressExportSetMax(const int &)));
    connect(pewWidget, SIGNAL(ProgressExportSetValue(const int &)), SLOT(on_pewWidget_ProgressExportSetValue(const int &)));
    connect(pewWidget, SIGNAL(VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)), SLOT(on_pewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)));
    connect(pewWidget, SIGNAL(VocabularyGetCategoryName(const int &, QString *)), SLOT(on_pewWidget_VocabularyGetCategoryName(const int &, QString *)));
    connect(pewWidget, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_pewWidget_VocabularyGetMarks(QStringList *)));
    connect(pewWidget, SIGNAL(VocabularyGetMarkText(const int &, const QString &, QString *)), SLOT(on_pewWidget_VocabularyGetMarkText(const int &, const QString &, QString *)));
    connect(pewWidget, SIGNAL(VocabularyGetRecordCount(const int &, int *)), SLOT(on_pewWidget_VocabularyGetRecordCount(const int &, int *)));
    connect(pewWidget, SIGNAL(VocabularyGetRecordIds(const int &, ExpInterface::tRecordIdList *)), SLOT(on_pewWidget_VocabularyGetRecordIds(const int &, ExpInterface::tRecordIdList *)));
} // SetupUI

Q_EXPORT_PLUGIN2(exp-plaintext, ExpPlaintext)