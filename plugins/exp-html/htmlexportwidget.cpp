#include "htmlexportwidget.h"

#include <QtGui/QTextTable>
#include <QtWidgets/QScrollBar>

HtmlExportWidget::HtmlExportWidget(QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QWidget(parent, flags)
{
	_ui.setupUi(this);

	_ui.codecs->setModel(&_codecsModel);
	preselectCodec("UTF-8");

	initTableColumns();
} // HtmlExportWidget

const QString HtmlExportWidget::getCodec() const
{
	QModelIndex index = _ui.codecs->currentIndex();
	return _codecsModel.data(index).toString();
} // getCodec

const QString HtmlExportWidget::getText() const
{
	if (_ui.styleText->isChecked())
	{
		return _ui.textPreview->toHtml();
	}
	else
	{
		return _ui.tablePreview->toHtml();
	} // if else
} // getText

const void HtmlExportWidget::refresh() const
{
	if (_ui.styleText->isChecked())
	{
		refreshText();
	}
	else
	{
		refreshTable();
	} // if else
} // refresh

const void HtmlExportWidget::addTableColumn()
{
    // controls
    TableColumn column;
    column.headerEdit   = new QLineEdit(_ui.pageTable);
    column.templateEdit = new QLineEdit(_ui.pageTable);
    column.width        = new QSpinBox(_ui.pageTable);
    column.width->setMaximum(COLUMN_MAX_WIDTH);
    column.width->setValue(COLUMN_DEFAULTWIDTH);
    column.headerWidget = new QWidget(_ui.pageTable);
    _tableColumns.append(column);

    // header
    QHBoxLayout *header = new QHBoxLayout(column.headerWidget);
    header->setContentsMargins(QMargins());
    header->addWidget(column.headerEdit);
    header->addWidget(column.width);
    _ui.tableColumns->addWidget(column.headerWidget, TableRowHeader, _tableColumns.size() + LABEL_COLUMN);
    // template
    _ui.tableColumns->addWidget(column.templateEdit, TableRowTemplate, _tableColumns.size() + LABEL_COLUMN);
} // addTableColumn

const void HtmlExportWidget::initTableColumns()
{
    for (quint8 column = 0; column < _ui.tableColums->value(); column++)
	{
        addTableColumn();
    } // for
} // initTableColumns

const void HtmlExportWidget::insertTableText(const QTextTable *tablePreview, const quint32 &row, const quint8 &column, const QString &text) const
{
    QTextTableCell cell = tablePreview->cellAt(row, column);
    QTextCursor cursor = cell.firstCursorPosition();
    cursor.insertText(text);
} // insertTableText

const void HtmlExportWidget::preselectCodec(const QString &codec) const
{
    for (quint8 codecIndex = 0; codecIndex < _codecsModel.rowCount(); codecIndex++)
	{
        QModelIndex index = _codecsModel.index(codecIndex, CodecsModel::ColumnCodec);
        if (codec == _codecsModel.data(index))
		{
            _ui.codecs->setCurrentIndex(index);
            return;
        } // if
    } // for
} // preselectCodec

const void HtmlExportWidget::refreshTable() const
{
    // prepare table
    _ui.tablePreview->clear();
    QTextCursor cursor = _ui.tablePreview->textCursor();
    // format
    QTextTableFormat tableFormat;
    QVector<QTextLength> widths;
    foreach (const TableColumn &column, _tableColumns)
	{
        widths.append(QTextLength(QTextLength::FixedLength, column.width->value()));
    } // foreach
    tableFormat.setColumnWidthConstraints(widths);
    QTextTable *tablePreview = cursor.insertTable(HEADER_ROW + 1, _tableColumns.size(), tableFormat);

    cursor.beginEditBlock();

    // header labels
    for (quint8 tableColumn = 0; tableColumn < _tableColumns.size(); tableColumn++)
	{
        insertTableText(tablePreview, HEADER_ROW, tableColumn, _tableColumns.at(tableColumn).headerEdit->text());
    } // for

    // categories
    ExpInterface::CategoryIdList categoryIds;
    emit vocabularyGetCategoryIds(&categoryIds);

    // total record count for progress
    quint32 totalRecords = 0;
    foreach (const quint8 &categoryId, categoryIds)
	{
        quint32 records;
        emit vocabularyGetRecordCount(categoryId, &records);
        totalRecords += records;
    } // foreach
    emit progressExportSetMax(totalRecords);
    QCoreApplication::processEvents(); // to avoid crash

    QStringList marks;
    emit vocabularyGetMarks(&marks);

    // preview
    bool firstLine  = true;
    quint32 records = 0;
    foreach (const quint8 &categoryId, categoryIds)
	{
        if (firstLine)
		{
            firstLine = false;
        }
		else
		{
            tablePreview->appendRows(1);
            tablePreview->mergeCells(tablePreview->rows() - 1, 0, 1, _tableColumns.size());
        } // if else

        QString categoryName;
        emit vocabularyGetCategoryName(categoryId, &categoryName);
        tablePreview->appendRows(1);
        quint32 tableRow = tablePreview->rows() - 1;
        tablePreview->mergeCells(tableRow, 0, 1, _tableColumns.size());
        insertTableText(tablePreview, tableRow, 0, categoryName);

        // records
        ExpInterface::RecordIdList recordIds;
        emit vocabularyGetRecordIds(categoryId, &recordIds);
        foreach (const quint32 &recordId, recordIds)
		{
            tablePreview->appendRows(1);
            tableRow = tablePreview->rows() - 1;

            for (quint8 column = 0; column < _tableColumns.size(); column++)
			{
                QString templateText = _tableColumns.at(column).templateEdit->text();

                // replace marks for data
                foreach (const QString &mark, marks)
				{
                    QString data;
                    emit vocabularyGetMarkText(recordId, mark, &data);
                    templateText.replace(mark, data);
                } // foreach

                insertTableText(tablePreview, tableRow, column, templateText);
            } // for

            records++;
            emit progressExportSetValue(records);
        } // foreach
    } // foreach

    cursor.endEditBlock();

    emit progressExportSetValue(0);
} // refreshTable

const void HtmlExportWidget::refreshText() const
{
    _ui.textPreview->clear();

    QTextCursor cursor = _ui.textPreview->textCursor();
    cursor.beginEditBlock();

    // categories
    ExpInterface::CategoryIdList categoryIds;
    emit vocabularyGetCategoryIds(&categoryIds);

    // total record count for progress
    quint32 totalRecords = 0;
    foreach (const quint8 &categoryId, categoryIds)
	{
        quint32 records;
        emit vocabularyGetRecordCount(categoryId, &records);
        totalRecords += records;
    } // foreach
    emit progressExportSetMax(totalRecords);

    QStringList marks;
    emit vocabularyGetMarks(&marks);

    // preview
    bool firstLine  = true;
    quint32 records = 0;
    foreach (const quint8 &categoryId, categoryIds)
	{
        if (firstLine)
		{
            firstLine = false;
        }
		else
		{
            _ui.textPreview->append("");
        } // if

        QString categoryName;
        emit vocabularyGetCategoryName(categoryId, &categoryName);
        _ui.textPreview->append(categoryName);

        // records
        ExpInterface::RecordIdList recordIds;
        emit vocabularyGetRecordIds(categoryId, &recordIds);
        foreach (const quint32 &recordId, recordIds)
		{
            QString templateText = _ui.qleTextEdit->text();

            // replace marks for data
            foreach (const QString &mark, marks)
			{
                QString data;
                emit vocabularyGetMarkText(recordId, mark, &data);
                templateText.replace(mark, data);
            } // foreach

            _ui.textPreview->append(templateText);

            records++;
            emit progressExportSetValue(records);
        } // foreach
    } // foreach

    cursor.endEditBlock();

    _ui.textPreview->verticalScrollBar()->setValue(0);

    emit progressExportSetValue(0);
} // refreshText

const void HtmlExportWidget::removeTableColumn()
{
    TableColumn column = _tableColumns.takeLast();
    // header
    column.headerEdit->deleteLater();
    column.width->deleteLater();
    column.headerWidget->deleteLater();
    // template
    column.templateEdit->deleteLater();
} // removeTableColumn

const void HtmlExportWidget::on_tableRefresh_clicked(bool checked /* false */) const
{
	refreshTable();
} // on_tableRefresh_clicked

const void HtmlExportWidget::on_textRefresh_clicked(bool checked /* false */) const
{
	refreshText();
} // on_textRefresh_clicked

const void HtmlExportWidget::on_styleTable_clicked(bool checked /* false */) const
{
	_ui.styles->setCurrentIndex(StyleTable);
} // on_styleTable_clicked

const void HtmlExportWidget::on_styleText_clicked(bool checked /* false */) const
{
	_ui.styles->setCurrentIndex(StyleText);
} // on_styleText_clicked

const void HtmlExportWidget::on_tableColums_valueChanged(int i)
{
	if (i < _tableColumns.size())
	{
		removeTableColumn();
	}
	else
	{
		addTableColumn();
	} // if else
} // on_tableColums_valueChanged