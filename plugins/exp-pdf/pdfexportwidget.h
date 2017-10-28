#ifndef PDFEXPORTWIDGET_H
#define PDFEXPORTWIDGET_H

#include <ui_pdfexportwidget.h>

#include "../common/exp-interface.h"
#include <hpdf_types.h>

class PdfExportWidget : public QWidget
{
  Q_OBJECT

  public:
    static const auto FONTROLE_NONE = -1;

    enum class EncodingSet
    {
      None,
      CNS,
      CNT,
      JPE = 0x4,
      KRE = 0x8
    };
    Q_DECLARE_FLAGS(EncodingSets, EncodingSet)
    enum class EncodingType
    {
      Singlebyte,
      Multibyte
    };
    enum class FontRole
    {
      Category,
      Template,
      Mark
    };
    enum class FontSet
    {
      None,
      CNS,
      CNT,
      JP = 0x4,
      KR = 0x8
    };
    Q_DECLARE_FLAGS(FontSets, FontSet)
    enum class Style
    {
      Text,
      Table
    };

    struct EncodingInfo
    {
      QString      name;
      EncodingType encodingType;
      EncodingSet  encodingSet;
      char        *textCodec;
    };
    struct FontInfo
    {
      QString      name;
      EncodingType encodingType;
      FontSet      fontSet;
    };
    struct FontRoleInfo
    {
      QString     font;
      FontSet     fontSet;
      QString     encoding;
      EncodingSet encodingSet;
      quintptr    size;
      const char *textCodec;
    };
    struct PageSize
    {
      QString        name;
      HPDF_PageSizes pdfPageSize;
    };
    struct TableColumn
    {
      QWidget   *headerWidget;
      QLineEdit *headerEdit;
      QSpinBox  *width;
      QLineEdit *templateEdit;
    };

    using TableColumns = QList<TableColumn>;

    PdfExportWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);

          quintptr            border       ()                                               const;
          quintptr            compression  ()                                               const;
          FontRoleInfo        fontRoleInfo (FontRole fontRole, qintptr num = FONTROLE_NONE) const;
          void                initMarkFonts();
          HPDF_PageSizes      pageSize     ()                                               const;
          Style               style        ()                                               const;
    const TableColumns       *tableColumns ()                                               const;
          QString             textTemplate ()                                               const;

  private:
    enum class TableRow
    {
      Header,
      Template
    };

    struct FontControls
    {
      QComboBox *font;
      QComboBox *encoding;
      QSpinBox  *size;
    };

    QList<FontControls> _fontControls;
    QList<TableColumn>  _tableColumns;
    Ui::PdfExportWidget _ui;

    virtual ~PdfExportWidget() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    void        addTableColumn   ();
    void        fillEncodings    (QComboBox *comboBox, const QString &font) const;
    void        fillFonts        (QComboBox *comboBox)                      const;
    void        fillPageSizes    ()                                         const;
    EncodingSet encodingSet      (const QString &encoding)                  const;
    FontSet     fontSet          (const QString &font)                      const;
    char       *textCodec        (const QString &encoding)                  const;
    void        initTableColumns ();
    void        removeTableColumn();

  Q_SIGNALS:
    void vocabularyGetMarks(QStringList *marks) const;

  private Q_SLOTS:
    void on_font_currentIndexChanged(int index)             const;
    void on_styleTable_clicked      (bool checked = false)  const;
    void on_styleText_clicked       (bool checked = false)  const;
    void on_tableColums_valueChanged(int i);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PdfExportWidget::EncodingSets)
Q_DECLARE_OPERATORS_FOR_FLAGS(PdfExportWidget::FontSets)

#endif