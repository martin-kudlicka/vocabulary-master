#ifndef EXP_PDF_H
#define EXP_PDF_H

#include "pdfexportwidget.h"
#include <hpdf.h>

class ExpPdf : public ExpInterface
{
	Q_OBJECT
	Q_INTERFACES(ExpInterface)

	private:
		static const int BORDER = 0;

		struct sFont {
			HPDF_Font hfFont;
			int iSize;
			const QTextCodec *qtcTextCodec;
		}; // sFont

		typedef QList<sFont> tFontList;

		PdfExportWidget *_pewWidget;

		const void AddFont(const HPDF_Doc &pPdf, tFontList *pFontList, const PdfExportWidget::eFontRole &pFontRole, const int &pNum = PdfExportWidget::FONTROLE_NONE) const;
		virtual const bool BeginExport() const;
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
