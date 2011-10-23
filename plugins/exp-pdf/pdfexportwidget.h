#ifndef PDFEXPORTWIDGET_H
#define PDFEXPORTWIDGET_H

#include <ui_pdfexportwidget.h>

#include "../common/exp-interface.h"

class PdfExportWidget : public QWidget
{
	Q_OBJECT

	public:
		PdfExportWidget(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

		const QString GetCategoryFont() const;
		const int GetCategoryFontSize() const;
		const QString GetRecordFont() const;
		const int GetRecordFontSize() const;
		const QString GetTextTemplate() const;

	private:
		enum eStyle {
			StyleText,
			StyleTable
		}; // eStyle
        enum eTableRow {
            TableRowHeader,
            TableRowTemplate
        }; // eTableRow

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

        QList<sTableColumn> _qlTableColumns;
		Ui::qwPdfExport _qwpePdfExport;

        const void AddTableColumn();
		const void FillFonts(QComboBox *pComboBox) const;
        const void InitTableColumns();
        const void RefreshTable() const;
        const void RefreshText() const;
        const void RemoveTableColumn();

    private slots:
		const void on_qrbStyleTable_clicked(bool checked = false) const;
        const void on_qrbStyleText_clicked(bool checked = false) const;
        const void on_qsbTableColums_valueChanged(int i);
}; // PdfExportWidget

#endif // PDFEXPORTWIDGET_H