#include "exp-pdf.h"

#include <QtGui/QFileDialog>
#include <QtCore/QTextCodec>

const void ExpPdf::AddFont(const HPDF_Doc &pPdf, tFontList *pFontList, const PdfExportWidget::eFontRole &pFontRole, const int &pNum /* PdfExportWidget::FONTROLE_NONE */) const
{
	PdfExportWidget::sFontRoleInfo sfriFontRoleInfo = _pewWidget->GetFontRoleInfo(pFontRole, pNum);

	sFont smfFont;
	smfFont.hfFont = HPDF_GetFont(pPdf, sfriFontRoleInfo.qsFont.toLocal8Bit(), sfriFontRoleInfo.qsEncoding.toLocal8Bit());
	smfFont.iSize = sfriFontRoleInfo.iSize;
	smfFont.qtcTextCodec = QTextCodec::codecForName(sfriFontRoleInfo.cTextCodec);
	pFontList->append(smfFont);
} // AddFont

const bool ExpPdf::BeginExport() const
{
	// get filename
	QString qsFile = QFileDialog::getSaveFileName(_pewWidget, QString(), QString(), tr("pdf (*.pdf)"));
	if (qsFile.isEmpty()) {
		return false;
	} // if

	// marks
	QStringList qslMarks;
	emit VocabularyGetMarks(&qslMarks);

	// PDF
	HPDF_Doc hdPdf = HPDF_New(NULL, NULL);
	//HPDF_SetCompressionMode(hdPdf, HPDF_COMP_ALL);

	// get font info with font and encoding set
	QList<sFont> qlFonts;
	InitFonts(hdPdf, &qlFonts, qslMarks.size());

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
	PdfAddPage(hdPdf, &hpPage, qlFonts.at(PdfExportWidget::FontRoleCategory).iSize);
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
		PdfSetFont(hpPage, qlFonts.at(PdfExportWidget::FontRoleCategory).hfFont, qlFonts.at(PdfExportWidget::FontRoleCategory).iSize);
		PdfShowText(hpPage, qsCategoryName, qlFonts.at(PdfExportWidget::FontRoleCategory).qtcTextCodec);

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
					PdfSetFont(hpPage, qlFonts.at(PdfExportWidget::FontRoleTemplate).hfFont, qlFonts.at(PdfExportWidget::FontRoleTemplate).iSize);
					QString qsText = qsTemplate.mid(iPos);
					PdfShowText(hpPage, qsText, qlFonts.at(PdfExportWidget::FontRoleTemplate).qtcTextCodec);

					break;
				} else {
					// text before possible mark
					if (iMarkPos > iPos) {
						PdfSetFont(hpPage, qlFonts.at(PdfExportWidget::FontRoleTemplate).hfFont, qlFonts.at(PdfExportWidget::FontRoleTemplate).iSize);
						QString qsText = qsTemplate.mid(iPos, iMarkPos - iPos);
						PdfShowText(hpPage, qsText, qlFonts.at(PdfExportWidget::FontRoleTemplate).qtcTextCodec);
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
							PdfSetFont(hpPage, qlFonts.at(PdfExportWidget::FontRoleMark + iMark).hfFont, qlFonts.at(PdfExportWidget::FontRoleMark + iMark).iSize);
							PdfShowText(hpPage, qsData, qlFonts.at(PdfExportWidget::FontRoleMark + iMark).qtcTextCodec);

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

const void ExpPdf::InitFonts(const HPDF_Doc &pPdf, tFontList *pFontList, const int &pMarkCount) const
{
	// get demanded fonts and encodings
	PdfExportWidget::sFontRoleInfo sfriFont = _pewWidget->GetFontRoleInfo(PdfExportWidget::FontRoleCategory);
	PdfExportWidget::qfFontSets qffsFontSets = sfriFont.efsFontSet;
	PdfExportWidget::qfEncodingSets qfesEncodingSets = sfriFont.eesEncodingSet;
	sfriFont = _pewWidget->GetFontRoleInfo(PdfExportWidget::FontRoleTemplate);
	qffsFontSets |= sfriFont.efsFontSet;
	qfesEncodingSets |= sfriFont.eesEncodingSet;
	for (int iMark = 0; iMark < pMarkCount; iMark++) {
		sfriFont = _pewWidget->GetFontRoleInfo(PdfExportWidget::FontRoleMark, iMark);
		qffsFontSets |= sfriFont.efsFontSet;
		qfesEncodingSets |= sfriFont.eesEncodingSet;
	} // for

	// enable demanded CID fonts
	if (qffsFontSets & PdfExportWidget::FontSetCNS) {
		HPDF_UseCNSFonts(pPdf);
	} // if
	if (qffsFontSets & PdfExportWidget::FontSetCNT) {
		HPDF_UseCNTFonts(pPdf);
	} // if
	if (qffsFontSets & PdfExportWidget::FontSetJP) {
		HPDF_UseJPFonts(pPdf);
	} // if
	if (qffsFontSets & PdfExportWidget::FontSetKR) {
		HPDF_UseKRFonts(pPdf);
	} // if

	// enable demanded encodings
	if (qfesEncodingSets & PdfExportWidget::EncodingSetCNS) {
		HPDF_UseCNSEncodings(pPdf);
	} // if
	if (qfesEncodingSets & PdfExportWidget::EncodingSetCNT) {
		HPDF_UseCNTEncodings(pPdf);
	} // if
	if (qfesEncodingSets & PdfExportWidget::EncodingSetJPE) {
		HPDF_UseJPEncodings(pPdf);
	} // if
	if (qfesEncodingSets & PdfExportWidget::EncodingSetKRE) {
		HPDF_UseKREncodings(pPdf);
	} // if

	// load fonts
	AddFont(pPdf, pFontList, PdfExportWidget::FontRoleCategory);
	AddFont(pPdf, pFontList, PdfExportWidget::FontRoleTemplate);
	for (int iMark = 0; iMark < pMarkCount; iMark++) {
		AddFont(pPdf, pFontList, PdfExportWidget::FontRoleMark, iMark);
	} // for
} // InitFonts

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
	HPDF_Page_MoveTextPos(*pPage, _pewWidget->GetBorder(), HPDF_Page_GetHeight(*pPage) - _pewWidget->GetBorder() - hrSize);

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
	if (hpPosition.y < hrSize + _pewWidget->GetBorder()) {
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

const void ExpPdf::PdfShowText(const HPDF_Page &pPage, const QString &pText, const QTextCodec *pTextCodec) const
{
	QByteArray qbaEncoded;
	if (pTextCodec) {
		qbaEncoded = pTextCodec->fromUnicode(pText);
	} else {
		qbaEncoded = pText.toAscii();
	} // if else

	HPDF_Page_ShowText(pPage, qbaEncoded);
} // PdfShowText

const void ExpPdf::SetupUI(QWidget *pParent)
{
	_pewWidget = new PdfExportWidget(pParent);
	QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
	pLayout->insertWidget(WIDGET_POSITION, _pewWidget);

	connect(_pewWidget, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_pewWidget_VocabularyGetMarks(QStringList *)));

	_pewWidget->InitMarkFonts();
} // SetupUI

Q_EXPORT_PLUGIN2(exp-pdf, ExpPdf)