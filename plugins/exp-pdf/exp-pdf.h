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

		struct sMarkFont {
			HPDF_Font hfFont;
			int iSize;
		}; // sMarkFont

		PdfExportWidget *_pewWidget;

		virtual const bool BeginExport() const;
		virtual const QString GetPluginName() const;
		const void PdfAddPage(const HPDF_Doc &pPdf, HPDF_Page *pPage, const HPDF_REAL &pDefaultSize = 0) const;
		const bool PdfNextLine(const HPDF_Doc &pPdf, HPDF_Page *pPage) const;
		const void PdfSetFont(const HPDF_Page &pPage, const HPDF_Font &pFont, const int &pSize) const;
		virtual const void SetupUI(QWidget *pParent);

	private slots:
		const void on_pewWidget_VocabularyGetMarks(QStringList *pMarks) const;
}; // ExpPdf

#endif // EXP_PDF_H
