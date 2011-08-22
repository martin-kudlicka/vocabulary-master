#include "plaintextexportwidget.h"

const QString PlaintextExportWidget::GetText() const
{
    return _qwpePlaintextExport.qptePlainPreview->toPlainText();
} // GetText

const void PlaintextExportWidget::on_qpbPlainRefresh_clicked(bool checked /* false */) const
{
    Refresh();
} // on_qpbPlainRefresh_clicked

PlaintextExportWidget::PlaintextExportWidget(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(pParent, pFlags)
{
    _qwpePlaintextExport.setupUi(this);
} // PlaintextExportWidget

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