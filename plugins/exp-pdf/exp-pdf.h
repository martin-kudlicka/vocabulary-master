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
			HPDF_Font         pdfFont;
			quint8            size;
			const QTextCodec *textCodec;
		}; // Font

		typedef QList<Font> FontList;

		PdfExportWidget *_widget;

		virtual ~ExpPdf();

		        void    addFont           (HPDF_Doc pdfDocument, FontList *fontList, PdfExportWidget::FontRole fontRole, qint8 num = PdfExportWidget::FONTROLE_NONE) const;
		virtual void    beginExport       ()                                                                                                                         const;
		        void    exportTable       (quint32 recordId, HPDF_Page pdfPage, const FontList &fontList, const QStringList &marks)                                  const;
		        void    exportText        (quint32 recordId, HPDF_Page pdfPage, const FontList &fontList, const QStringList &marks, const QString &templateText)     const;
		        void    initFonts         (HPDF_Doc pdfDocument, FontList *fontList, quint8 markCount)                                                               const;
		        void    pdfAddPage        (HPDF_Doc pdfDocument, HPDF_Page *pdfPage, HPDF_REAL defaultSize = 0)                                                      const;
		        bool    pdfNextLine       (HPDF_Doc pdfDocument, HPDF_Page *pdfPage)                                                                                 const;
		        void    pdfSetFont        (HPDF_Page pdfPage, HPDF_Font pdfFont, quint8 size)                                                                        const;
		        void    pdfShowTableHeader(HPDF_Page pdfPage, const FontList &fontList)                                                                              const;
		        void    pdfShowText       (HPDF_Page pdfPage, const QString &text, const QTextCodec *textCodec)                                                      const;
		virtual QString pluginName        ()                                                                                                                         const;
		virtual void    setupUi           (QWidget *parent);

	private slots:
		void on_widget_vocabularyGetMarks(QStringList *marks) const;
}; // ExpPdf

#endif // EXP_PDF_H
