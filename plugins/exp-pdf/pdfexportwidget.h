#ifndef PDFEXPORTWIDGET_H
#define PDFEXPORTWIDGET_H

#include <ui_pdfexportwidget.h>

#include "../common/exp-interface.h"
#include "../common/codecsmodel.h"

class PdfExportWidget : public QWidget
{
	Q_OBJECT

	public:
		PdfExportWidget(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

        const QString GetCodec() const;
        const QString GetText() const;
        const void Refresh() const;

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

        CodecsModel _cmCodecsModel;
        QList<sTableColumn> _qlTableColumns;
		Ui::qwPdfExport _qwpePdfExport;

        const void AddTableColumn();
        const void InitTableColumns();
        const void InsertTableText(const QTextTable *pTablePreview, const int &pRow, const int &pColumn, const QString &pText) const;
        const void PreselectCodec(const QString &pCodec) const;
        const void RefreshTable() const;
        const void RefreshText() const;
        const void RemoveTableColumn();

    signals:
        void ProgressExportSetMax(const int &pMax) const;
        void ProgressExportSetValue(const int &pValue) const;
        void VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const;
        void VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const;
        void VocabularyGetMarks(QStringList *pMarks) const;
        void VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const;
        void VocabularyGetRecordCount(const int &pCategoryId, int *pCount) const;
        void VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const;

    private slots:
        const void on_qpbTableRefresh_clicked(bool checked = false) const;
        const void on_qpbTextRefresh_clicked(bool checked = false) const;
		const void on_qrbStyleTable_clicked(bool checked = false) const;
        const void on_qrbStyleText_clicked(bool checked = false) const;
        const void on_qsbTableColums_valueChanged(int i);
}; // PdfExportWidget

#endif // PDFEXPORTWIDGET_H