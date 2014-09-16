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

        QString codec  () const;
        void    refresh() const;
		QString text   () const;

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

        void addTableColumn   ();
        void initTableColumns ();
        void insertTableText  (const QTextTable *tablePreview, quint32 row, quint8 column, const QString &text) const;
        void preselectCodec   (const QString &codec)                                                            const;
        void refreshTable     ()                                                                                const;
        void refreshText      ()                                                                                const;
        void removeTableColumn();

    signals:
        void progressExportSetMax     (quint32 max)                                              const;
        void progressExportSetValue   (quint32 value)                                            const;
        void vocabularyGetCategoryIds (ExpInterface::CategoryIdList *categoryIds)                const;
        void vocabularyGetCategoryName(quint8 categoryId, QString *name)                         const;
        void vocabularyGetMarks       (QStringList *marks)                                       const;
        void vocabularyGetMarkText    (quint32 recordId, const QString &mark, QString *text)     const;
        void vocabularyGetRecordCount (quint8 categoryId, quint32 *count)                        const;
        void vocabularyGetRecordIds   (quint8 categoryId, ExpInterface::RecordIdList *recordIds) const;

    private slots:
		void on_styleTable_clicked      (bool checked = false) const;
        void on_styleText_clicked       (bool checked = false) const;
		void on_tableColums_valueChanged(int i);
		void on_tableRefresh_clicked    (bool checked = false) const;
		void on_textRefresh_clicked     (bool checked = false) const;
}; // HtmlExportWidget

#endif // HTMLEXPORTWIDGET_H