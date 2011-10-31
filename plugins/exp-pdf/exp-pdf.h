#ifndef EXP_PDF_H
#define EXP_PDF_H

#include "pdfexportwidget.h"
#include <hpdf_doc.h>

class ExpPdf : public ExpInterface
{
	Q_OBJECT
	Q_INTERFACES(ExpInterface)

	private:
		struct sFont {
			HPDF_Font hfFont;
			int iSize;
			const QTextCodec *qtcTextCodec;
		}; // sFont

		typedef QList<sFont> tFontList;

		PdfExportWidget *_pewWidget;

		const void AddFont(const HPDF_Doc &pPdf, tFontList *pFontList, const PdfExportWidget::eFontRole &pFontRole, const int &pNum = PdfExportWidget::FONTROLE_NONE) const;
		virtual const void BeginExport() const;
		const void ExportTable(const int &pRecordId, const HPDF_Page &pPage, const tFontList &pFontList, const QStringList &pMarks) const;
		const void ExportText(const int &pRecordId, const HPDF_Page &pPage, const tFontList &pFontList, const QStringList &pMarks, const QString &pTemplate) const;
		virtual const QString GetPluginName() const;
		const void InitFonts(const HPDF_Doc &pPdf, tFontList *pFontList, const int &pMarkCount) const;
		const void PdfAddPage(const HPDF_Doc &pPdf, HPDF_Page *pPage, const HPDF_REAL &pDefaultSize = 0) const;
		const bool PdfNextLine(const HPDF_Doc &pPdf, HPDF_Page *pPage) const;
		const void PdfSetFont(const HPDF_Page &pPage, const HPDF_Font &pFont, const int &pSize) const;
		const void PdfShowText(const HPDF_Page &pPage, const QString &pText, const QTextCodec *pTextCodec) const;
		virtual const void SetupUI(QWidget *pParent);

	private slots:
		const void on_pewWidget_VocabularyGetMarks(QStringList *pMarks) const;
}; // ExpPdf

#endif // EXP_PDF_H
