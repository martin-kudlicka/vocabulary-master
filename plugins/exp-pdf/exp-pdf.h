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
    struct Font
    {
      HPDF_Font         pdfFont;
      quintptr          size;
      const QTextCodec *textCodec;
    };

    typedef QList<Font> FontList;

    PdfExportWidget *_widget;

    virtual ~ExpPdf() override;

    void addFont           (HPDF_Doc pdfDocument, FontList *fontList, PdfExportWidget::FontRole fontRole, qintptr num = PdfExportWidget::FONTROLE_NONE) const;
    void exportTable       (quintptr recordId, HPDF_Page pdfPage, const FontList &fontList, const QStringList &marks)                                   const;
    void exportText        (quintptr recordId, HPDF_Page pdfPage, const FontList &fontList, const QStringList &marks, const QString &templateText)      const;
    void initFonts         (HPDF_Doc pdfDocument, FontList *fontList, qintptr markCount)                                                                const;
    void pdfAddPage        (HPDF_Doc pdfDocument, HPDF_Page *pdfPage, HPDF_REAL defaultSize = 0)                                                        const;
    bool pdfNextLine       (HPDF_Doc pdfDocument, HPDF_Page *pdfPage)                                                                                   const;
    void pdfSetFont        (HPDF_Page pdfPage, HPDF_Font pdfFont, quintptr size)                                                                        const;
    void pdfShowTableHeader(HPDF_Page pdfPage, const FontList &fontList)                                                                                const;
    void pdfShowText       (HPDF_Page pdfPage, const QString &text, const QTextCodec *textCodec)                                                        const;

    virtual void    beginExport()                const override;
    virtual QString pluginName ()                const override;
    virtual void    setupUi    (QWidget *parent)       override;

  private slots:
    void on_widget_vocabularyGetMarks(QStringList *marks) const;
};

#endif