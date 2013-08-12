#include "htmlexportwidget.h"

#include <QtGui/QTextTable>
#include <QtWidgets/QScrollBar>

const void HtmlExportWidget::AddTableColumn()
{
    // controls
    sTableColumn stcColumn;
    stcColumn.qleHeader = new QLineEdit(_qwheHtmlExport.qwPageTable);
    stcColumn.qleTemplate = new QLineEdit(_qwheHtmlExport.qwPageTable);
    stcColumn.qsbWidth = new QSpinBox(_qwheHtmlExport.qwPageTable);
    stcColumn.qsbWidth->setMaximum(COLUMN_MAX_WIDTH);
    stcColumn.qsbWidth->setValue(COLUMN_DEFAULTWIDTH);
    stcColumn.qwHeader = new QWidget(_qwheHtmlExport.qwPageTable);
    _qlTableColumns.append(stcColumn);

    // header
    QHBoxLayout *qhblHeader = new QHBoxLayout(stcColumn.qwHeader);
    qhblHeader->setContentsMargins(QMargins());
    qhblHeader->addWidget(stcColumn.qleHeader);
    qhblHeader->addWidget(stcColumn.qsbWidth);
    _qwheHtmlExport.qglTableColumns->addWidget(stcColumn.qwHeader, TableRowHeader, _qlTableColumns.size() + LABEL_COLUMN);
    // template
    _qwheHtmlExport.qglTableColumns->addWidget(stcColumn.qleTemplate, TableRowTemplate, _qlTableColumns.size() + LABEL_COLUMN);
} // AddTableColumn

const QString HtmlExportWidget::GetCodec() const
{
    QModelIndex qmiIndex = _qwheHtmlExport.qtvCodecs->currentIndex();
    return _cmCodecsModel.data(qmiIndex).toString();
} // GetCodec

const QString HtmlExportWidget::GetText() const
{
    if (_qwheHtmlExport.qrbStyleText->isChecked()) {
        return _qwheHtmlExport.qteTextPreview->toHtml();
    } else {
        return _qwheHtmlExport.qteTablePreview->toHtml();
    } // if else
} // GetText

const void HtmlExportWidget::InitTableColumns()
{
    for (int iColumn = 0; iColumn < _qwheHtmlExport.qsbTableColums->value(); iColumn++) {
        AddTableColumn();
    } // for
} // InitTableColumns

const void HtmlExportWidget::InsertTableText(const QTextTable *pTablePreview, const int &pRow, const int &pColumn, const QString &pText) const
{
    QTextTableCell qttcCell = pTablePreview->cellAt(pRow, pColumn);
    QTextCursor qtcCursor = qttcCell.firstCursorPosition();
    qtcCursor.insertText(pText);
} // InsertTableText

const void HtmlExportWidget::on_qpbTableRefresh_clicked(bool checked /* false */) const
{
    RefreshTable();
} // on_qpbTableRefresh_clicked

const void HtmlExportWidget::on_qpbTextRefresh_clicked(bool checked /* false */) const
{
    RefreshText();
} // on_qpbTextRefresh_clicked

const void HtmlExportWidget::on_qrbStyleTable_clicked(bool checked /* false */) const
{
	_qwheHtmlExport.qswStyles->setCurrentIndex(StyleTable);
} // on_qrbStyleTable_clicked

const void HtmlExportWidget::on_qrbStyleText_clicked(bool checked /* false */) const
{
    _qwheHtmlExport.qswStyles->setCurrentIndex(StyleText);
} // on_qrbStyleText_clicked

const void HtmlExportWidget::on_qsbTableColums_valueChanged(int i)
{
    if (i < _qlTableColumns.size()) {
        RemoveTableColumn();
    } else {
        AddTableColumn();
    } // if else
} // on_qsbTableColums_valueChanged

HtmlExportWidget::HtmlExportWidget(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(pParent, pFlags)
{
    _qwheHtmlExport.setupUi(this);

    _qwheHtmlExport.qtvCodecs->setModel(&_cmCodecsModel);
    PreselectCodec("UTF-8");

    InitTableColumns();
} // HtmlExportWidget

const void HtmlExportWidget::PreselectCodec(const QString &pCodec) const
{
    for (int iI = 0; iI < _cmCodecsModel.rowCount(); iI++) {
        QModelIndex qmiIndex = _cmCodecsModel.index(iI, CodecsModel::ColumnCodec);
        if (pCodec == _cmCodecsModel.data(qmiIndex)) {
            _qwheHtmlExport.qtvCodecs->setCurrentIndex(qmiIndex);
            return;
        } // if
    } // for
} // PreselectCodec

const void HtmlExportWidget::Refresh() const
{
    if (_qwheHtmlExport.qrbStyleText->isChecked()) {
        RefreshText();
    } else {
        RefreshTable();
    } // if else
} // Refresh

const void HtmlExportWidget::RefreshTable() const
{
    // prepare table
    _qwheHtmlExport.qteTablePreview->clear();
    QTextCursor qtcCursor = _qwheHtmlExport.qteTablePreview->textCursor();
    // format
    QTextTableFormat qttfTableFormat;
    QVector<QTextLength> qvWidths;
    foreach (sTableColumn stcColumn, _qlTableColumns) {
        qvWidths.append(QTextLength(QTextLength::FixedLength, stcColumn.qsbWidth->value()));
    } // foreach
    qttfTableFormat.setColumnWidthConstraints(qvWidths);
    QTextTable *qttTablePreview = qtcCursor.insertTable(HEADER_ROW + 1, _qlTableColumns.size(), qttfTableFormat);

    qtcCursor.beginEditBlock();

    // header labels
    for (int iI = 0; iI < _qlTableColumns.size(); iI++) {
        InsertTableText(qttTablePreview, HEADER_ROW, iI, _qlTableColumns.at(iI).qleHeader->text());
    } // for

    // categories
    ExpInterface::tCategoryIdList tcilCategoryIds;
    emit VocabularyGetCategoryIds(&tcilCategoryIds);

    // total record count for progress
    int iTotalRecords = 0;
    foreach (int iCategoryId, tcilCategoryIds) {
        int iRecords;
        emit VocabularyGetRecordCount(iCategoryId, &iRecords);
        iTotalRecords += iRecords;
    } // foreach
    emit ProgressExportSetMax(iTotalRecords);
    QCoreApplication::processEvents();  // to avoid crash

    QStringList qslMarks;
    emit VocabularyGetMarks(&qslMarks);

    // preview
    bool bFirstLine = true;
    int iRecords = 0;
    foreach (int iCategoryId, tcilCategoryIds) {
        if (bFirstLine) {
            bFirstLine = false;
        } else {
            qttTablePreview->appendRows(1);
            qttTablePreview->mergeCells(qttTablePreview->rows() - 1, 0, 1, _qlTableColumns.size());
        } // if

        QString qsCategoryName;
        emit VocabularyGetCategoryName(iCategoryId, &qsCategoryName);
        qttTablePreview->appendRows(1);
        int iTableRow = qttTablePreview->rows() - 1;
        qttTablePreview->mergeCells(iTableRow, 0, 1, _qlTableColumns.size());
        InsertTableText(qttTablePreview, iTableRow, 0, qsCategoryName);

        // records
        ExpInterface::tRecordIdList trilRecordIds;
        emit VocabularyGetRecordIds(iCategoryId, &trilRecordIds);
        foreach (int iRecordId, trilRecordIds) {
            qttTablePreview->appendRows(1);
            iTableRow = qttTablePreview->rows() - 1;

            for (int iColumn = 0; iColumn < _qlTableColumns.size(); iColumn++) {
                QString qsTemplate = _qlTableColumns.at(iColumn).qleTemplate->text();

                // replace marks for data
                foreach (QString qsMark, qslMarks) {
                    QString qsData;
                    emit VocabularyGetMarkText(iRecordId, qsMark, &qsData);
                    qsTemplate.replace(qsMark, qsData);
                } // foreach

                InsertTableText(qttTablePreview, iTableRow, iColumn, qsTemplate);
            } // for

            iRecords++;
            emit ProgressExportSetValue(iRecords);
        } // foreach
    } // foreach

    qtcCursor.endEditBlock();

    emit ProgressExportSetValue(0);
} // RefreshTable

const void HtmlExportWidget::RefreshText() const
{
    _qwheHtmlExport.qteTextPreview->clear();

    QTextCursor qtcCursor = _qwheHtmlExport.qteTextPreview->textCursor();
    qtcCursor.beginEditBlock();

    // categories
    ExpInterface::tCategoryIdList tcilCategoryIds;
    emit VocabularyGetCategoryIds(&tcilCategoryIds);

    // total record count for progress
    int iTotalRecords = 0;
    foreach (int iCategoryId, tcilCategoryIds) {
        int iRecords;
        emit VocabularyGetRecordCount(iCategoryId, &iRecords);
        iTotalRecords += iRecords;
    } // foreach
    emit ProgressExportSetMax(iTotalRecords);

    QStringList qslMarks;
    emit VocabularyGetMarks(&qslMarks);

    // preview
    bool bFirstLine = true;
    int iRecords = 0;
    foreach (int iCategoryId, tcilCategoryIds) {
        if (bFirstLine) {
            bFirstLine = false;
        } else {
            _qwheHtmlExport.qteTextPreview->append("");
        } // if

        QString qsCategoryName;
        emit VocabularyGetCategoryName(iCategoryId, &qsCategoryName);
        _qwheHtmlExport.qteTextPreview->append(qsCategoryName);

        // records
        ExpInterface::tRecordIdList trilRecordIds;
        emit VocabularyGetRecordIds(iCategoryId, &trilRecordIds);
        foreach (int iRecordId, trilRecordIds) {
            QString qsTemplate = _qwheHtmlExport.qleTextEdit->text();

            // replace marks for data
            foreach (QString qsMark, qslMarks) {
                QString qsData;
                emit VocabularyGetMarkText(iRecordId, qsMark, &qsData);
                qsTemplate.replace(qsMark, qsData);
            } // foreach

            _qwheHtmlExport.qteTextPreview->append(qsTemplate);

            iRecords++;
            emit ProgressExportSetValue(iRecords);
        } // foreach
    } // foreach

    qtcCursor.endEditBlock();

    _qwheHtmlExport.qteTextPreview->verticalScrollBar()->setValue(0);

    emit ProgressExportSetValue(0);
} // RefreshText

const void HtmlExportWidget::RemoveTableColumn()
{
    sTableColumn stcColumn = _qlTableColumns.takeLast();
    // header
    stcColumn.qleHeader->deleteLater();
    stcColumn.qsbWidth->deleteLater();
    stcColumn.qwHeader->deleteLater();
    // template
    stcColumn.qleTemplate->deleteLater();
} // RemoveTableColumn