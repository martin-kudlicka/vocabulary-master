#include "htmlexportwidget.h"

#include <QtGui/QTextTable>
#include <QtWidgets/QScrollBar>

Q_DECL_CONSTEXPR auto COLUMN_DEFAULTWIDTH = 100;
Q_DECL_CONSTEXPR auto COLUMN_MAX_WIDTH    = 999;
Q_DECL_CONSTEXPR auto HEADER_ROW          = 0;
Q_DECL_CONSTEXPR auto LABEL_COLUMN        = 1;

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
  const auto modelIndex = _ui.codecs->currentIndex();
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
  QString previewText;

  if (_ui.styleText->isChecked())
  {
    previewText = _ui.textPreview->toHtml();
  }
  else
  {
    previewText = _ui.tablePreview->toHtml();
  }

  return previewText;
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

  {
    // header
    auto hBoxHeader = new QHBoxLayout(tableColumn.headerWidget);
    hBoxHeader->setContentsMargins(QMargins());
    hBoxHeader->addWidget(tableColumn.headerEdit);
    hBoxHeader->addWidget(tableColumn.width);
  }

  _ui.tableColumns->addWidget(tableColumn.headerWidget, static_cast<int>(TableRow::Header), _tableColumns.size() + LABEL_COLUMN);
  // template
  _ui.tableColumns->addWidget(tableColumn.templateEdit, static_cast<int>(TableRow::Template), _tableColumns.size() + LABEL_COLUMN);
}

void HtmlExportWidget::initTableColumns()
{
  for (auto columnIndex = 0; columnIndex < _ui.tableColums->value(); columnIndex++)
  {
    addTableColumn();
  }
}

void HtmlExportWidget::insertTableText(const QTextTable *tablePreview, quintptr row, quintptr column, const QString &text) const
{
  const auto cell = tablePreview->cellAt(row, column);
  auto cursor     = cell.firstCursorPosition();
  cursor.insertText(text);
}

void HtmlExportWidget::preselectCodec(const QString &codec) const
{
  for (auto codecIndex = 0; codecIndex < _codecsModel.rowCount(); codecIndex++)
  {
    const auto modelIndex = _codecsModel.index(codecIndex, static_cast<int>(CodecsModel::Column::Codec));
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

  {
    auto textCursor = _ui.tablePreview->textCursor();

    {
      // format
      QTextTableFormat textTableFormat;
      QVector<QTextLength> tableWidths;
      for (const auto &columnIndex : _tableColumns)
      {
        tableWidths.append(QTextLength(QTextLength::FixedLength, columnIndex.width->value()));
      }
      textTableFormat.setColumnWidthConstraints(tableWidths);
      auto textTable = textCursor.insertTable(HEADER_ROW + 1, _tableColumns.size(), textTableFormat);

      textCursor.beginEditBlock();

      // header labels
      for (auto columnIndex = 0; columnIndex < _tableColumns.size(); columnIndex++)
      {
        insertTableText(textTable, HEADER_ROW, columnIndex, _tableColumns.at(columnIndex).headerEdit->text());
      }

      // categories
      ExpInterface::CategoryIdList categoryIds;
      emit vocabularyGetCategoryIds(&categoryIds);

      // total record count for progress
      {
        auto totalRecords = 0;
        for (auto categoryId : categoryIds)
        {
          quintptr records;
          emit vocabularyGetRecordCount(categoryId, &records);
          totalRecords += records;
        }
        emit progressExportSetMax(totalRecords);
      }
      QCoreApplication::processEvents(); // to avoid crash

      QStringList marks;
      emit vocabularyGetMarks(&marks);

      // preview
      auto firstLine = true;
      auto records   = 0;
      for (auto categoryId : categoryIds)
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
        auto tableRow = textTable->rows() - 1;
        textTable->mergeCells(tableRow, 0, 1, _tableColumns.size());
        insertTableText(textTable, tableRow, 0, categoryName);

        // records
        ExpInterface::RecordIdList recordIds;
        emit vocabularyGetRecordIds(categoryId, &recordIds);
        for (auto recordId : recordIds)
        {
          textTable->appendRows(1);
          tableRow = textTable->rows() - 1;

          for (auto column = 0; column < _tableColumns.size(); column++)
          {
            auto templateText = _tableColumns.at(column).templateEdit->text();

            // replace marks for data
            for (const auto &mark : marks)
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
    }

    textCursor.endEditBlock();
  }

  emit progressExportSetValue(0);
}

void HtmlExportWidget::refreshText() const
{
  _ui.textPreview->clear();

  {
    auto textCursor = _ui.textPreview->textCursor();
    textCursor.beginEditBlock();

    // categories
    ExpInterface::CategoryIdList categoryIds;
    emit vocabularyGetCategoryIds(&categoryIds);

    {
      // total record count for progress
      auto totalRecords = 0;
      for (auto categoryId : categoryIds)
      {
        quintptr records;
        emit vocabularyGetRecordCount(categoryId, &records);
        totalRecords += records;
      }
      emit progressExportSetMax(totalRecords);
    }

    QStringList marks;
    emit vocabularyGetMarks(&marks);

    // preview
    auto firstLine = true;
    auto records   = 0;
    for (auto categoryId : categoryIds)
    {
      if (firstLine)
      {
        firstLine = false;
      }
      else
      {
        _ui.textPreview->append("");
      }

      {
        QString categoryName;
        emit vocabularyGetCategoryName(categoryId, &categoryName);
        _ui.textPreview->append(categoryName);
      }

      // records
      ExpInterface::RecordIdList recordIds;
      emit vocabularyGetRecordIds(categoryId, &recordIds);
      for (auto recordId : recordIds)
      {
        {
          auto templateText = _ui.qleTextEdit->text();

          // replace marks for data
          for (const auto &mark : marks)
          {
            QString data;
            emit vocabularyGetMarkText(recordId, mark, &data);
            templateText.replace(mark, data);
          }

          _ui.textPreview->append(templateText);
        }

        records++;
        emit progressExportSetValue(records);
      }
    }

    textCursor.endEditBlock();
  }

  _ui.textPreview->verticalScrollBar()->setValue(0);

  emit progressExportSetValue(0);
}

void HtmlExportWidget::removeTableColumn()
{
  auto tableColumn = _tableColumns.takeLast();
  // header
  tableColumn.headerEdit->deleteLater();
  tableColumn.width->deleteLater();
  tableColumn.headerWidget->deleteLater();
  // template
  tableColumn.templateEdit->deleteLater();
}

void HtmlExportWidget::on_styleTable_clicked(bool checked /* false */) const
{
  _ui.styles->setCurrentIndex(static_cast<int>(Style::Table));
}

void HtmlExportWidget::on_styleText_clicked(bool checked /* false */) const
{
  _ui.styles->setCurrentIndex(static_cast<int>(Style::Text));
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