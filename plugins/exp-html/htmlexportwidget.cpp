#include "htmlexportwidget.h"

#include <QtGui/QTextTable>
#include <QtWidgets/QScrollBar>

HtmlExportWidget::HtmlExportWidget(QWidget *parent /* nullptr */, Qt::WindowFlags flags /* 0 */) : QWidget(parent, flags)
{
	_ui.setupUi(this);

	_ui.codecs->setModel(&_codecsModel);
	preselectCodec("UTF-8");

	initTableColumns();
}

HtmlExportWidget::~HtmlExportWidget()
{
}

QString HtmlExportWidget::codec() const
{
	const QModelIndex modelIndex = _ui.codecs->currentIndex();
	return _codecsModel.data(modelIndex).toString();
}

void HtmlExportWidget::refresh() const
{
	if (_ui.styleText->isChecked())
	{
		refreshText();
	}
	else
	{
		refreshTable();
	}
}

QString HtmlExportWidget::text() const
{
	if (_ui.styleText->isChecked())
	{
		return _ui.textPreview->toHtml();
	}
	else
	{
		return _ui.tablePreview->toHtml();
	}
}

void HtmlExportWidget::addTableColumn()
{
    // controls
    TableColumn tableColumn;
    tableColumn.headerEdit   = new QLineEdit(_ui.pageTable);
    tableColumn.templateEdit = new QLineEdit(_ui.pageTable);
    tableColumn.width        = new QSpinBox(_ui.pageTable);
    tableColumn.width->setMaximum(COLUMN_MAX_WIDTH);
    tableColumn.width->setValue(COLUMN_DEFAULTWIDTH);
    tableColumn.headerWidget = new QWidget(_ui.pageTable);
    _tableColumns.append(tableColumn);

    // header
    QHBoxLayout *hBoxHeader = new QHBoxLayout(tableColumn.headerWidget);
    hBoxHeader->setContentsMargins(QMargins());
    hBoxHeader->addWidget(tableColumn.headerEdit);
    hBoxHeader->addWidget(tableColumn.width);
    _ui.tableColumns->addWidget(tableColumn.headerWidget, TableRowHeader, _tableColumns.size() + LABEL_COLUMN);
    // template
    _ui.tableColumns->addWidget(tableColumn.templateEdit, TableRowTemplate, _tableColumns.size() + LABEL_COLUMN);
}

void HtmlExportWidget::initTableColumns()
{
    for (quint8 columnIndex = 0; columnIndex < _ui.tableColums->value(); columnIndex++)
	{
        addTableColumn();
    }
}

void HtmlExportWidget::insertTableText(const QTextTable *tablePreview, quint32 row, quint8 column, const QString &text) const
{
    const QTextTableCell cell = tablePreview->cellAt(row, column);
    QTextCursor cursor        = cell.firstCursorPosition();
    cursor.insertText(text);
}

void HtmlExportWidget::preselectCodec(const QString &codec) const
{
    for (quint8 codecIndex = 0; codecIndex < _codecsModel.rowCount(); codecIndex++)
	{
        const QModelIndex modelIndex = _codecsModel.index(codecIndex, CodecsModel::ColumnCodec);
        if (codec == _codecsModel.data(modelIndex))
		{
            _ui.codecs->setCurrentIndex(modelIndex);
            return;
        }
    }
}

void HtmlExportWidget::refreshTable() const
{
    // prepare table
    _ui.tablePreview->clear();
    QTextCursor textCursor = _ui.tablePreview->textCursor();
    // format
    QTextTableFormat textTableFormat;
    QVector<QTextLength> tableWidths;
    for (const TableColumn &columnIndex : _tableColumns)
	{
        tableWidths.append(QTextLength(QTextLength::FixedLength, columnIndex.width->value()));
    }
    textTableFormat.setColumnWidthConstraints(tableWidths);
    QTextTable *textTable = textCursor.insertTable(HEADER_ROW + 1, _tableColumns.size(), textTableFormat);

    textCursor.beginEditBlock();

    // header labels
    for (quint8 columnIndex = 0; columnIndex < _tableColumns.size(); columnIndex++)
	{
        insertTableText(textTable, HEADER_ROW, columnIndex, _tableColumns.at(columnIndex).headerEdit->text());
    }

    // categories
    ExpInterface::CategoryIdList categoryIds;
    emit vocabularyGetCategoryIds(&categoryIds);

    // total record count for progress
    quint32 totalRecords = 0;
    for (const quint8 &categoryId : categoryIds)
	{
        quint32 records;
        emit vocabularyGetRecordCount(categoryId, &records);
        totalRecords += records;
    }
    emit progressExportSetMax(totalRecords);
    QCoreApplication::processEvents(); // to avoid crash

    QStringList marks;
    emit vocabularyGetMarks(&marks);

    // preview
    bool firstLine  = true;
    quint32 records = 0;
    for (const quint8 &categoryId : categoryIds)
	{
        if (firstLine)
		{
            firstLine = false;
        }
		else
		{
            textTable->appendRows(1);
            textTable->mergeCells(textTable->rows() - 1, 0, 1, _tableColumns.size());
        }

        QString categoryName;
        emit vocabularyGetCategoryName(categoryId, &categoryName);
        textTable->appendRows(1);
        quint32 tableRow = textTable->rows() - 1;
        textTable->mergeCells(tableRow, 0, 1, _tableColumns.size());
        insertTableText(textTable, tableRow, 0, categoryName);

        // records
        ExpInterface::RecordIdList recordIds;
        emit vocabularyGetRecordIds(categoryId, &recordIds);
        for (const quint32 &recordId : recordIds)
		{
            textTable->appendRows(1);
            tableRow = textTable->rows() - 1;

            for (quint8 column = 0; column < _tableColumns.size(); column++)
			{
                QString templateText = _tableColumns.at(column).templateEdit->text();

                // replace marks for data
                for (const QString &mark : marks)
				{
                    QString data;
                    emit vocabularyGetMarkText(recordId, mark, &data);
                    templateText.replace(mark, data);
                }

                insertTableText(textTable, tableRow, column, templateText);
            }

            records++;
            emit progressExportSetValue(records);
        }
    }

    textCursor.endEditBlock();

    emit progressExportSetValue(0);
}

void HtmlExportWidget::refreshText() const
{
    _ui.textPreview->clear();

    QTextCursor textCursor = _ui.textPreview->textCursor();
    textCursor.beginEditBlock();

    // categories
    ExpInterface::CategoryIdList categoryIds;
    emit vocabularyGetCategoryIds(&categoryIds);

    // total record count for progress
    quint32 totalRecords = 0;
    for (const quint8 &categoryId : categoryIds)
	{
        quint32 records;
        emit vocabularyGetRecordCount(categoryId, &records);
        totalRecords += records;
    }
    emit progressExportSetMax(totalRecords);

    QStringList marks;
    emit vocabularyGetMarks(&marks);

    // preview
    bool firstLine  = true;
    quint32 records = 0;
    for (const quint8 &categoryId : categoryIds)
	{
        if (firstLine)
		{
            firstLine = false;
        }
		else
		{
            _ui.textPreview->append("");
        }

        QString categoryName;
        emit vocabularyGetCategoryName(categoryId, &categoryName);
        _ui.textPreview->append(categoryName);

        // records
        ExpInterface::RecordIdList recordIds;
        emit vocabularyGetRecordIds(categoryId, &recordIds);
        for (const quint32 &recordId : recordIds)
		{
            QString templateText = _ui.qleTextEdit->text();

            // replace marks for data
            for (const QString &mark : marks)
			{
                QString data;
                emit vocabularyGetMarkText(recordId, mark, &data);
                templateText.replace(mark, data);
            }

            _ui.textPreview->append(templateText);

            records++;
            emit progressExportSetValue(records);
        }
    }

    textCursor.endEditBlock();

    _ui.textPreview->verticalScrollBar()->setValue(0);

    emit progressExportSetValue(0);
}

void HtmlExportWidget::removeTableColumn()
{
    TableColumn tableColumn = _tableColumns.takeLast();
    // header
    tableColumn.headerEdit->deleteLater();
    tableColumn.width->deleteLater();
    tableColumn.headerWidget->deleteLater();
    // template
    tableColumn.templateEdit->deleteLater();
}

void HtmlExportWidget::on_styleTable_clicked(bool checked /* false */) const
{
	_ui.styles->setCurrentIndex(StyleTable);
}

void HtmlExportWidget::on_styleText_clicked(bool checked /* false */) const
{
	_ui.styles->setCurrentIndex(StyleText);
}

void HtmlExportWidget::on_tableColums_valueChanged(int i)
{
	if (i < _tableColumns.size())
	{
		removeTableColumn();
	}
	else
	{
		addTableColumn();
	}
}

void HtmlExportWidget::on_tableRefresh_clicked(bool checked /* false */) const
{
	refreshTable();
}

void HtmlExportWidget::on_textRefresh_clicked(bool checked /* false */) const
{
	refreshText();
}