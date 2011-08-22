#include "plaintextexportwidget.h"

const void PlaintextExportWidget::on_qpbPlainRefresh_clicked(bool checked /* false */) const
{
    QStringList qslMarks;
    emit VocabularyGetMarks(&qslMarks);
    ExpInterface::tCategoryIdList tcilCategoryIds;
    emit VocabularyGetCategoryIds(&tcilCategoryIds);

    foreach (int iCategoryId, tcilCategoryIds) {
        QString qsCategoryName;
        emit VocabularyGetCategoryName(iCategoryId, &qsCategoryName);
    } // foreach
} // on_qpbPlainRefresh_clicked

PlaintextExportWidget::PlaintextExportWidget(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(pParent, pFlags)
{
    _qwpePlaintextExport.setupUi(this);
} // PlaintextExportWidget