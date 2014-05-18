#ifndef HTMLEXPORTWIDGET_H
#define HTMLEXPORTWIDGET_H

#include <ui_htmlexportwidget.h>

#include "../common/exp-interface.h"
#include "../common/codecsmodel.h"

class HtmlExportWidget : public QWidget
{
	Q_OBJECT

	public:
		HtmlExportWidget(QWidget *parent = NULL, Qt::WindowFlags flags = 0);

        const QString codec  () const;
        const void    refresh() const;
		const QString text   () const;

	private:
		enum Style
		{
			StyleText,
			StyleTable
		}; // Style
        enum TableRow
		{
            TableRowHeader,
            TableRowTemplate
        }; // TableRow

        struct TableColumn
		{
            QWidget   *headerWidget;
            QLineEdit *headerEdit;
            QSpinBox  *width;
            QLineEdit *templateEdit;
        }; // TableColumn

        static const quint8  COLUMN_DEFAULTWIDTH = 100;
        static const quint16 COLUMN_MAX_WIDTH    = 999;
        static const quint8  HEADER_ROW          = 0;
        static const quint8  LABEL_COLUMN        = 1;

        CodecsModel          _codecsModel;
        QList<TableColumn>   _tableColumns;
		Ui::HtmlExportWidget _ui;

		virtual ~HtmlExportWidget();

        const void addTableColumn   ();
        const void initTableColumns ();
        const void insertTableText  (const QTextTable *tablePreview, const quint32 &row, const quint8 &column, const QString &text) const;
        const void preselectCodec   (const QString &codec)                                                                          const;
        const void refreshTable     ()                                                                                              const;
        const void refreshText      ()                                                                                              const;
        const void removeTableColumn();

    signals:
        void progressExportSetMax     (const quint32 &max)                                              const;
        void progressExportSetValue   (const quint32 &value)                                            const;
        void vocabularyGetCategoryIds (ExpInterface::CategoryIdList *categoryIds)                       const;
        void vocabularyGetCategoryName(const quint8 &categoryId, QString *name)                         const;
        void vocabularyGetMarks       (QStringList *marks)                                              const;
        void vocabularyGetMarkText    (const quint32 &recordId, const QString &mark, QString *text)     const;
        void vocabularyGetRecordCount (const quint8 &categoryId, quint32 *count)                        const;
        void vocabularyGetRecordIds   (const quint8 &categoryId, ExpInterface::RecordIdList *recordIds) const;

    private slots:
		const void on_styleTable_clicked      (bool checked = false) const;
        const void on_styleText_clicked       (bool checked = false) const;
		const void on_tableColums_valueChanged(int i);
		const void on_tableRefresh_clicked    (bool checked = false) const;
		const void on_textRefresh_clicked     (bool checked = false) const;
}; // HtmlExportWidget

#endif // HTMLEXPORTWIDGET_H