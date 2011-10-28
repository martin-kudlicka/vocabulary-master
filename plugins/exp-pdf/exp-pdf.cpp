#include "exp-pdf.h"

#include <QtGui/QFileDialog>

const bool ExpPdf::BeginExport() const
{
	// get filename
	QString qsFile = QFileDialog::getSaveFileName(_pewWidget, QString(), QString(), tr("pdf (*.pdf)"));
	if (qsFile.isEmpty()) {
		return false;
	} // if

	QStringList qslMarks;
	emit VocabularyGetMarks(&qslMarks);

	// PDF
	HPDF_Doc hdPdf = HPDF_New(NULL, NULL);
	//HPDF_SetCompressionMode(hdPdf, HPDF_COMP_ALL);

	// fonts
	PdfExportWidget::sFontRoleInfo sfriCategoryFont = _pewWidget->GetFontInfo(PdfExportWidget::FontRoleCategory);
	HPDF_Font hfCategory = HPDF_GetFont(hdPdf, sfriCategoryFont.qsFont.toLocal8Bit(), NULL);
	PdfExportWidget::sFontRoleInfo sfriTemplateFont = _pewWidget->GetFontInfo(PdfExportWidget::FontRoleTemplate);
	HPDF_Font hfTemplate = HPDF_GetFont(hdPdf, sfriTemplateFont.qsFont.toLocal8Bit(), NULL);
	QList<sMarkFont> qlMarkFonts;
	for (int iMark = 0; iMark < qslMarks.size(); iMark++) {
		PdfExportWidget::sFontRoleInfo sfriFont = _pewWidget->GetFontInfo(PdfExportWidget::FontRoleMark, iMark);

		sMarkFont smfFont;
		smfFont.hfFont = HPDF_GetFont(hdPdf, sfriFont.qsFont.toLocal8Bit(), NULL);
		smfFont.iSize = sfriFont.iSize;

		qlMarkFonts.append(smfFont);
	} // for

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

	// export
	HPDF_Page hpPage = NULL;
	PdfAddPage(hdPdf, &hpPage, sfriCategoryFont.iSize);
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
		PdfSetFont(hpPage, hfCategory, sfriCategoryFont.iSize);
		HPDF_Page_ShowText(hpPage, qsCategoryName.toLocal8Bit());

        // records
        ExpInterface::tRecordIdList trilRecordIds;
        emit VocabularyGetRecordIds(iCategoryId, &trilRecordIds);
        foreach (int iRecordId, trilRecordIds) {
            QString qsTemplate = _pewWidget->GetTextTemplate();

			PdfNextLine(hdPdf, &hpPage);

			// analyze template
			int iPos = 0;
			while (iPos < qsTemplate.size()) {
				int iMarkPos = qsTemplate.indexOf('$', iPos);

				if (iMarkPos == -1) {
					// no other mark on the line
					PdfSetFont(hpPage, hfTemplate, sfriTemplateFont.iSize);
					QString qsText = qsTemplate.mid(iPos);
					HPDF_Page_ShowText(hpPage, qsText.toLocal8Bit());

					break;
				} else {
					// text before possible mark
					if (iMarkPos > iPos) {
						PdfSetFont(hpPage, hfTemplate, sfriTemplateFont.iSize);
						QString qsText = qsTemplate.mid(iPos, iMarkPos - iPos);
						HPDF_Page_ShowText(hpPage, qsText.toLocal8Bit());
					} // if
					iPos = iMarkPos;

					// check if valid mark
					for (int iMark = 0; iMark < qslMarks.size(); iMark++) {
						QString qsMark = qslMarks.at(iMark);
						if (qsTemplate.mid(iMarkPos, qsMark.size()) == qsMark) {
							// valid mark, replace marks for data
							QString qsData;
							emit VocabularyGetMarkText(iRecordId, qsMark, &qsData);

							// show data
							PdfSetFont(hpPage, qlMarkFonts.at(iMark).hfFont, qlMarkFonts.at(iMark).iSize);
							HPDF_Page_ShowText(hpPage, qsData.toLocal8Bit());

							iPos += qsMark.size() - 1;
							break;
						} // if
					} // for

					iPos++;
				} // if else
			} // while

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

const void ExpPdf::on_pewWidget_VocabularyGetMarks(QStringList *pMarks) const
{
	emit VocabularyGetMarks(pMarks);
} // on_pewWidget_VocabularyGetMarks

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

	connect(_pewWidget, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_pewWidget_VocabularyGetMarks(QStringList *)));

	_pewWidget->InitMarkFonts();
} // SetupUI

Q_EXPORT_PLUGIN2(exp-pdf, ExpPdf)