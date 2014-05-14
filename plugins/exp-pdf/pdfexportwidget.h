#ifndef PDFEXPORTWIDGET_H
#define PDFEXPORTWIDGET_H

#include <ui_pdfexportwidget.h>

#include "../common/exp-interface.h"
#include <hpdf_types.h>

class PdfExportWidget : public QWidget
{
	Q_OBJECT

	public:
		static const qint8 FONTROLE_NONE = -1;

		enum EncodingSet
		{
			EncodingSetNone,
			EncodingSetCNS,
			EncodingSetCNT,
			EncodingSetJPE = 0x4,
			EncodingSetKRE = 0x8
		}; // EncodingSet
		Q_DECLARE_FLAGS(EncodingSets, EncodingSet)
		enum EncodingType
		{
			EncodingTypeSinglebyte,
			EncodingTypeMultibyte
		}; // EncodingType
		enum FontRole
		{
			FontRoleCategory,
			FontRoleTemplate,
			FontRoleMark
		}; // FontRole
		enum FontSet
		{
			FontSetNone,
			FontSetCNS,
			FontSetCNT,
			FontSetJP = 0x4,
			FontSetKR = 0x8
		}; // FontSet
		Q_DECLARE_FLAGS(FontSets, FontSet)
		enum Style
		{
			StyleText,
			StyleTable
		}; // Style

		struct EncodingInfo
		{
			QString      name;
			EncodingType encodingType;
			EncodingSet  encodingSet;
			char        *textCodec;
		}; // EncodingInfo
		struct FontInfo
		{
			QString      name;
			EncodingType encodingType;
			FontSet      fontSet;
		}; // FontInfo
		struct FontRoleInfo
		{
			QString     font;
			FontSet     fontSet;
			QString     encoding;
			EncodingSet encodingSet;
			quint8      size;
			const char *textCodec;
		}; // FontRoleInfo
		struct PageSize
		{
			QString        name;
			HPDF_PageSizes size;
		}; // PageSize
		struct TableColumn
		{
			QWidget   *headerWidget;
			QLineEdit *headerEdit;
			QSpinBox  *width;
			QLineEdit *templateEdit;
		}; // TableColumn

		typedef QList<TableColumn> TableColumns;

		PdfExportWidget(QWidget *parent = NULL, Qt::WindowFlags flags = 0);

		const quint8         getBorder      ()                                                       const;
		const quint8         getCompression ()                                                       const;
		const FontRoleInfo   getFontRoleInfo(const FontRole &role, const qint8 &num = FONTROLE_NONE) const;
		const HPDF_PageSizes getPageSize    ()                                                       const;
		const Style          getStyle       ()                                                       const;
		const TableColumns  *getTableColumns()                                                       const;
		const QString        getTextTemplate()                                                       const;
		const void           initMarkFonts  ();

	private:
		static const quint8 DEFAULT_FONT_COUNT = 2;

		enum FontDetails
		{
			FontDetailsFont,
			FontDetailsEncoding,
			FontDetailsSize
		}; // FontDetails
        enum TableRow
		{
            TableRowHeader,
            TableRowTemplate
        }; // TableRow

		struct FontControls
		{
			QComboBox *font;
			QComboBox *encoding;
			QSpinBox  *size;
		}; // FontControls

        static const quint8  COLUMN_DEFAULTWIDTH = 100;
        static const quint16 COLUMN_MAX_WIDTH    = 999;
        static const quint8  LABEL_COLUMN        = 1;

		QList<FontControls> _fontControls;
        QList<TableColumn>  _tableColumns;
		Ui::PdfExportWidget _ui;

        const void        addTableColumn   ();
		const void        fillEncodings    (QComboBox *comboBox, const QString &font) const;
		const void        fillFonts        (QComboBox *comboBox)                      const;
		const void        fillPageSizes    ()                                         const;
		const EncodingSet getEncodingSet   (const QString &encoding)                  const;
		const FontSet     getFontSet       (const QString &font)                      const;
		const char       *getTextCodec     (const QString &encoding)                  const;
        const void        initTableColumns ();
        const void        refreshTable     ()                                         const;
        const void        refreshText      ()                                         const;
        const void        removeTableColumn();

	signals:
		void vocabularyGetMarks(QStringList *marks) const;

    private slots:
		const void on_font_currentIndexChanged(int index)            const;
		const void on_styleTable_clicked      (bool checked = false) const;
        const void on_styleText_clicked       (bool checked = false) const;
        const void on_tableColums_valueChanged(int i);
}; // PdfExportWidget

Q_DECLARE_OPERATORS_FOR_FLAGS(PdfExportWidget::EncodingSets)
Q_DECLARE_OPERATORS_FOR_FLAGS(PdfExportWidget::FontSets)

#endif // PDFEXPORTWIDGET_H