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
			HPDF_PageSizes pdfPageSize;
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

		      quint8         border       ()                                             const;
		      quint8         compression  ()                                             const;
		      FontRoleInfo   fontRoleInfo (FontRole fontRole, qint8 num = FONTROLE_NONE) const;
		      void           initMarkFonts();
		      HPDF_PageSizes pageSize     ()                                             const;
		      Style          style        ()                                             const;
		const TableColumns  *tableColumns ()                                             const;
		      QString        textTemplate ()                                             const;

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

		virtual ~PdfExportWidget() override;

        void        addTableColumn   ();
		void        fillEncodings    (QComboBox *comboBox, const QString &font) const;
		void        fillFonts        (QComboBox *comboBox)                      const;
		void        fillPageSizes    ()                                         const;
		EncodingSet encodingSet      (const QString &encoding)                  const;
		FontSet     fontSet          (const QString &font)                      const;
		char       *textCodec        (const QString &encoding)                  const;
        void        initTableColumns ();
        void        removeTableColumn();

	signals:
		void vocabularyGetMarks(QStringList *marks) const;

    private slots:
		void on_font_currentIndexChanged(int index)            const;
		void on_styleTable_clicked      (bool checked = false) const;
        void on_styleText_clicked       (bool checked = false) const;
        void on_tableColums_valueChanged(int i);
}; // PdfExportWidget

Q_DECLARE_OPERATORS_FOR_FLAGS(PdfExportWidget::EncodingSets)
Q_DECLARE_OPERATORS_FOR_FLAGS(PdfExportWidget::FontSets)

#endif // PDFEXPORTWIDGET_H