#include "exp-pdf.h"

#include <QtGui/QFileDialog>

const bool ExpPdf::BeginExport() const
{
	// get filename
	QString qsFile = QFileDialog::getSaveFileName(_pewWidget, QString(), QString(), tr("pdf (*.pdf)"));
	if (qsFile.isEmpty()) {
		return false;
	} // if

	// PDF
	HPDF_Doc hdPdf = HPDF_New(NULL, NULL);
	//HPDF_SetCompressionMode(hdPdf, HPDF_COMP_ALL);

	// fonts
	HPDF_Font hfCategory = HPDF_GetFont(hdPdf, _pewWidget->GetCategoryFont().toLocal8Bit(), NULL);
	HPDF_Font hfRecord = HPDF_GetFont(hdPdf, _pewWidget->GetRecordFont().toLocal8Bit(), NULL);

	// categories
	ExpInterface::tCategoryIdList tcilCategoryIds;
	emit VocabularyGetCategoryIds(&tcilCategoryIds);

	// total record count for progress
	int iTotalRecords = 0;
	foreach (int iCategoryId, tcilCategoryIds) {
		int iRecords;
		emit VocabularyGetRecordCount(iCategoryId, &iRecords);
		iTotalRecords += iRecords;
	} // foreach
	emit ProgressExportSetMax(iTotalRecords);

	QStringList qslMarks;
	emit VocabularyGetMarks(&qslMarks);

	// export
	HPDF_Page hpPage = NULL;
	PdfAddPage(hdPdf, &hpPage, _pewWidget->GetCategoryFontSize());
	PdfSetFont(hpPage, hfCategory, _pewWidget->GetCategoryFontSize());
	bool bFirstLine = true;
	int iRecords = 0;
	foreach (int iCategoryId, tcilCategoryIds) {
        if (bFirstLine) {
            bFirstLine = false;
        } else {
			if (!PdfNextLine(hdPdf, &hpPage)) {
				PdfNextLine(hdPdf, &hpPage);
			} // if
        } // if

		// category
        QString qsCategoryName;
        emit VocabularyGetCategoryName(iCategoryId, &qsCategoryName);
		HPDF_Page_ShowText(hpPage, qsCategoryName.toLocal8Bit());

        // records
		PdfSetFont(hpPage, hfRecord, _pewWidget->GetRecordFontSize());
        ExpInterface::tRecordIdList trilRecordIds;
        emit VocabularyGetRecordIds(iCategoryId, &trilRecordIds);
        foreach (int iRecordId, trilRecordIds) {
            QString qsTemplate = _pewWidget->GetTextTemplate();

            // replace marks for data
            foreach (QString qsMark, qslMarks) {
                QString qsData;
                emit VocabularyGetMarkText(iRecordId, qsMark, &qsData);
                qsTemplate.replace(qsMark, qsData);
            } // foreach

			PdfNextLine(hdPdf, &hpPage);
			HPDF_Page_ShowText(hpPage, qsTemplate.toLocal8Bit());

            iRecords++;
            emit ProgressExportSetValue(iRecords);
        } // foreach
    } // foreach

	HPDF_SaveToFile(hdPdf, qsFile.toLocal8Bit());

	emit ProgressExportSetValue(0);
	HPDF_Free(hdPdf);

	return true;
} // BeginExport

const QString ExpPdf::GetPluginName() const
{
	return tr("pdf (pdf)");
} // GetPluginName

const void ExpPdf::PdfAddPage(const HPDF_Doc &pPdf, HPDF_Page *pPage, const HPDF_REAL &pDefaultSize /* 0 */) const
{
	// remember current font and size
	HPDF_Font hfFont;
	HPDF_REAL hrSize;
	if (*pPage) {
		hfFont = HPDF_Page_GetCurrentFont(*pPage);
		hrSize = HPDF_Page_GetCurrentFontSize(*pPage);
	} else {
		hfFont = NULL;
		hrSize = pDefaultSize;
	} // if else

	*pPage = HPDF_AddPage(pPdf);
	HPDF_Page_BeginText(*pPage);
	HPDF_Page_MoveTextPos(*pPage, BORDER, HPDF_Page_GetHeight(*pPage) - BORDER - hrSize);

	// set previous font and size
	if (hfFont) {
		PdfSetFont(*pPage, hfFont, hrSize);
	} // if
} // PdfAddPage

const bool ExpPdf::PdfNextLine(const HPDF_Doc &pPdf, HPDF_Page *pPage) const
{
	// check current position
	HPDF_REAL hrSize = HPDF_Page_GetCurrentFontSize(*pPage);
	HPDF_Point hpPosition = HPDF_Page_GetCurrentTextPos(*pPage);
	if (hpPosition.y < hrSize + BORDER) {
		PdfAddPage(pPdf, pPage);
		return true;
	} else {
		HPDF_Page_MoveToNextLine(*pPage);
		return false;
	} // if else
} // PdfNextLine

const void ExpPdf::PdfSetFont(const HPDF_Page &pPage, const HPDF_Font &pFont, const int &pSize) const
{
	HPDF_Page_SetFontAndSize(pPage, pFont, pSize);
	HPDF_Page_SetTextLeading(pPage, pSize);
} // PdfSetFont

const void ExpPdf::SetupUI(QWidget *pParent)
{
	_pewWidget = new PdfExportWidget(pParent);
	QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
	pLayout->insertWidget(WIDGET_POSITION, _pewWidget);
} // SetupUI

Q_EXPORT_PLUGIN2(exp-pdf, ExpPdf)