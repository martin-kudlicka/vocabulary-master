#include "plaintextexportwidget.h"

const QString PlaintextExportWidget::GetCodec() const
{
    QModelIndex qmiIndex = _qwpePlaintextExport.qtvCodecs->currentIndex();
    return _cmCodecsModel.data(qmiIndex).toString();
} // GetCodec

const QString PlaintextExportWidget::GetText() const
{
    return _qwpePlaintextExport.qptePlainPreview->toPlainText();
} // GetText

const void PlaintextExportWidget::InitTableColumns()
{
    for (int iColumn = 0; iColumn < _qwpePlaintextExport.qsbTableColums->value(); iColumn++) {
        sTableColumn stcColumn;
        stcColumn.qleHeader = new QLineEdit(_qwpePlaintextExport.qwPageTable);
        stcColumn.qleTemplate = new QLineEdit(_qwpePlaintextExport.qwPageTable);
        _qlTableColumns.append(stcColumn);

        _qwpePlaintextExport.qglTableColumns->addWidget(stcColumn.qleHeader, TableRowHeader, iColumn + 1);
        _qwpePlaintextExport.qglTableColumns->addWidget(stcColumn.qleTemplate, TableRowTemplate, iColumn + 1);
    } // for
} // InitTableColumns

const void PlaintextExportWidget::on_qpbPlainRefresh_clicked(bool checked /* false */) const
{
    Refresh();
} // on_qpbPlainRefresh_clicked

const void PlaintextExportWidget::on_qrbStylePlain_clicked(bool checked /* false */) const
{
	_qwpePlaintextExport.qswStyles->setCurrentIndex(StylePlain);
} // on_qrbStylePlain_clicked

const void PlaintextExportWidget::on_qrbStyleTable_clicked(bool checked /* false */) const
{
	_qwpePlaintextExport.qswStyles->setCurrentIndex(StyleTable);
} // on_qrbStyleTable_clicked

PlaintextExportWidget::PlaintextExportWidget(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(pParent, pFlags)
{
    _qwpePlaintextExport.setupUi(this);

    _qwpePlaintextExport.qtvCodecs->setModel(&_cmCodecsModel);
    PreselectCodec("UTF-8");

    InitTableColumns();
} // PlaintextExportWidget

const void PlaintextExportWidget::PreselectCodec(const QString &pCodec) const
{
    for (int iI = 0; iI < _cmCodecsModel.rowCount(); iI++) {
        QModelIndex qmiIndex = _cmCodecsModel.index(iI, CodecsModel::ColumnCodec);
        if (pCodec == _cmCodecsModel.data(qmiIndex)) {
            _qwpePlaintextExport.qtvCodecs->setCurrentIndex(qmiIndex);
            return;
        } // if
    } // for
} // PreselectCodec

const void PlaintextExportWidget::Refresh() const
{
    _qwpePlaintextExport.qptePlainPreview->clear();

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
            _qwpePlaintextExport.qptePlainPreview->appendPlainText("");
            bFirstLine = false;
        } // if

        QString qsCategoryName;
        emit VocabularyGetCategoryName(iCategoryId, &qsCategoryName);
        _qwpePlaintextExport.qptePlainPreview->appendPlainText(qsCategoryName);

        // records
        ExpInterface::tRecordIdList trilRecordIds;
        emit VocabularyGetRecordIds(iCategoryId, &trilRecordIds);
        foreach (int iRecordId, trilRecordIds) {
            QString qsTemplate = _qwpePlaintextExport.qlePlainEdit->text();

            // replace marks for data
            foreach (QString qsMark, qslMarks) {
                QString qsData;
                emit VocabularyGetMarkText(iRecordId, qsMark, &qsData);
                qsTemplate.replace(qsMark, qsData);
            } // foreach

            _qwpePlaintextExport.qptePlainPreview->appendPlainText(qsTemplate);

            iRecords++;
            emit ProgressExportSetValue(iRecords);
        } // foreach
    } // foreach

    emit ProgressExportSetValue(0);
} // Refresh