#ifndef PDFEXPORTWIDGET_H
#define PDFEXPORTWIDGET_H

#include <ui_pdfexportwidget.h>

#include "../common/exp-interface.h"

class PdfExportWidget : public QWidget
{
	Q_OBJECT
	Q_ENUMS(eEncodingSet)
	Q_ENUMS(eEncodingType)

	public:
		enum eEncodingSet {
			EncodingSetNone,
			EncodingSetCNS,
			EncodingSetCNT,
			EncodingSetJPE = 4,
			EncodingSetKRE = 8
		}; // eEncodingSet
		Q_DECLARE_FLAGS(qfEncodingSets, eEncodingSet)
		enum eEncodingType {
			EncodingTypeSinglebyte,
			EncodingTypeMultibyte
		}; // eEncodingType
		enum eFontRole {
			FontRoleCategory,
			FontRoleTemplate,
			FontRoleMark
		}; // eFontRole

		struct sEncodingInfo {
			QString qsName;
			eEncodingType eetEncodingType;
			eEncodingSet eesEncodingSet;
		}; // sEncodingInfo
		struct sFontInfo {
			QString qsName;
			eEncodingType eetEncoding;
		}; // sFontInfo
		struct sFontRoleInfo {
			QString qsFont;
			QString qsEncoding;
			eEncodingSet eesEncodingSet;
			int iSize;
		}; // sFontRoleInfo

		PdfExportWidget(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

		sFontRoleInfo GetFontInfo(const eFontRole &pRole, const int &pNum = -1) const;
		const QString GetTextTemplate() const;
		const void InitMarkFonts();

	private:
		static const int DEFAULT_FONT_COUNT = 2;

		enum eFontDetails {
			FontDetailsFont,
			FontDetailsEncoding,
			FontDetailsSize
		}; // eFontDetails
		enum eStyle {
			StyleText,
			StyleTable
		}; // eStyle
        enum eTableRow {
            TableRowHeader,
            TableRowTemplate
        }; // eTableRow

		struct sFontControls {
			QComboBox *qcbFont;
			QComboBox *qcbEncoding;
			QSpinBox *qsbSize;
		}; // sFontControls
        struct sTableColumn {
            QWidget *qwHeader;
            QLineEdit *qleHeader;
            QSpinBox *qsbWidth;
            QLineEdit *qleTemplate;
        }; // sTableColumn

        static const int COLUMN_DEFAULTWIDTH = 100;
        static const int COLUMN_MAX_WIDTH = 999;
        static const int HEADER_ROW = 0;
        static const int LABEL_COLUMN = 1;

		QList<sFontControls> _qlFontControls;
        QList<sTableColumn> _qlTableColumns;
		Ui::qwPdfExport _qwpePdfExport;

        const void AddTableColumn();
		const void FillEncodings(QComboBox *pComboBox, const QString &pFont) const;
		const void FillFonts(QComboBox *pComboBox) const;
        const void InitTableColumns();
        const void RefreshTable() const;
        const void RefreshText() const;
        const void RemoveTableColumn();

	signals:
		void VocabularyGetMarks(QStringList *pMarks) const;

    private slots:
		const void on_qcbFont_currentIndexChanged(int index) const;
		const void on_qrbStyleTable_clicked(bool checked = false) const;
        const void on_qrbStyleText_clicked(bool checked = false) const;
        const void on_qsbTableColums_valueChanged(int i);
}; // PdfExportWidget

Q_DECLARE_OPERATORS_FOR_FLAGS(PdfExportWidget::qfEncodingSets)

#endif // PDFEXPORTWIDGET_H