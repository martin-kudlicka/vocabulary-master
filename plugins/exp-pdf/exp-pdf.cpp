#include "exp-pdf.h"

#include <hpdf.h>

const bool ExpPdf::BeginExport() const
{
	HPDF_Doc hdPdf = HPDF_New(NULL, NULL);

	// TODO

	HPDF_Free(hdPdf);
	return true;
} // BeginExport

const QString ExpPdf::GetPluginName() const
{
	return tr("pdf (pdf)");
} // GetPluginName

const void ExpPdf::on_pewWidget_ProgressExportSetMax(const int &pMax) const
{
    emit ProgressExportSetMax(pMax);
} // on_pewWidget_ProgressExportSetMax

const void ExpPdf::on_pewWidget_ProgressExportSetValue(const int &pValue) const
{
    emit ProgressExportSetValue(pValue);
} // on_pewWidget_ProgressExportSetValue

const void ExpPdf::on_pewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const
{
    emit VocabularyGetCategoryIds(pCategoryIds);
} // on_pewWidget_VocabularyGetCategoryIds

const void ExpPdf::on_pewWidget_VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const
{
    emit VocabularyGetCategoryName(pCategoryId, pName);
} // on_pewWidget_VocabularyGetCategoryName

const void ExpPdf::on_pewWidget_VocabularyGetMarks(QStringList *pMarks) const
{
    emit VocabularyGetMarks(pMarks);
} // on_pewWidget_VocabularyGetMarks

const void ExpPdf::on_pewWidget_VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const
{
    emit VocabularyGetMarkText(pRecordId, pMark, pText);
} // on_pewWidget_VocabularyGetMarkText

const void ExpPdf::on_pewWidget_VocabularyGetRecordCount(const int &pCategoryId, int *pCount) const
{
    emit VocabularyGetRecordCount(pCategoryId, pCount);
} // on_pewWidget_VocabularyGetRecordCount

const void ExpPdf::on_pewWidget_VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const
{
    emit VocabularyGetRecordIds(pCategoryId, pRecordIds);
} // on_pewWidget_VocabularyGetRecordCount

const void ExpPdf::SetupUI(QWidget *pParent)
{
	_pewWidget = new PdfExportWidget(pParent);
	QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
	pLayout->insertWidget(WIDGET_POSITION, _pewWidget);

	connect(_pewWidget, SIGNAL(ProgressExportSetMax(const int &)), SLOT(on_pewWidget_ProgressExportSetMax(const int &)));
	connect(_pewWidget, SIGNAL(ProgressExportSetValue(const int &)), SLOT(on_pewWidget_ProgressExportSetValue(const int &)));
	connect(_pewWidget, SIGNAL(VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)), SLOT(on_pewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)));
	connect(_pewWidget, SIGNAL(VocabularyGetCategoryName(const int &, QString *)), SLOT(on_pewWidget_VocabularyGetCategoryName(const int &, QString *)));
	connect(_pewWidget, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_pewWidget_VocabularyGetMarks(QStringList *)));
	connect(_pewWidget, SIGNAL(VocabularyGetMarkText(const int &, const QString &, QString *)), SLOT(on_pewWidget_VocabularyGetMarkText(const int &, const QString &, QString *)));
	connect(_pewWidget, SIGNAL(VocabularyGetRecordCount(const int &, int *)), SLOT(on_pewWidget_VocabularyGetRecordCount(const int &, int *)));
	connect(_pewWidget, SIGNAL(VocabularyGetRecordIds(const int &, ExpInterface::tRecordIdList *)), SLOT(on_pewWidget_VocabularyGetRecordIds(const int &, ExpInterface::tRecordIdList *)));
} // SetupUI

Q_EXPORT_PLUGIN2(exp-pdf, ExpPdf)