#ifndef EXP_PDF_H
#define EXP_PDF_H

#include "pdfexportwidget.h"
#include <hpdf_doc.h>

class ExpPdf : public ExpInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID IID_EXPINTERFACE FILE "exp-pdf.json")
	Q_INTERFACES(ExpInterface)

	private:
		static const qint8 RECORD_NONE = -1;

		struct Font
		{
			HPDF_Font         font;
			quint8            size;
			const QTextCodec *textCodec;
		}; // Font

		typedef QList<Font> FontList;

		PdfExportWidget *_widget;

		        const void    addFont           (const HPDF_Doc &pdf, FontList *fontList, const PdfExportWidget::FontRole &fontRole, const qint8 &num = PdfExportWidget::FONTROLE_NONE) const;
		virtual const void    beginExport       ()                                                                                                                                       const;
		        const void    exportTable       (const quint32 &recordId, const HPDF_Page &page, const FontList &fontList, const QStringList &marks)                                     const;
		        const void    exportText        (const quint32 &recordId, const HPDF_Page &page, const FontList &fontList, const QStringList &marks, const QString &templateText)        const;
		virtual const QString getPluginName     ()                                                                                                                                       const;
		        const void    initFonts         (const HPDF_Doc &pdf, FontList *fontList, const quint8 &markCount)                                                                       const;
		        const void    pdfAddPage        (const HPDF_Doc &pdf, HPDF_Page *page, const HPDF_REAL &defaultSize = 0)                                                                 const;
		        const bool    pdfNextLine       (const HPDF_Doc &pdf, HPDF_Page *page)                                                                                                   const;
		        const void    pdfSetFont        (const HPDF_Page &page, const HPDF_Font &font, const quint8 &size)                                                                       const;
		        const void    pdfShowTableHeader(const HPDF_Page &page, const FontList &fontList)                                                                                        const;
		        const void    pdfShowText       (const HPDF_Page &page, const QString &text, const QTextCodec *textCodec)                                                                const;
		virtual const void    setupUi           (QWidget *parent);

	private slots:
		const void on_widget_vocabularyGetMarks(QStringList *marks) const;
}; // ExpPdf

#endif // EXP_PDF_H
