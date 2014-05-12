#include "exp-plaintext.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>

const void ExpPlaintext::beginExport() const
{
    // get filename
    QString qsFile = QFileDialog::getSaveFileName(_pewWidget, QString(), QString(), tr("plaintext (*.txt)"));
    if (qsFile.isEmpty()) {
        return;
    } // if

    // open file
    QFile qfFile(qsFile);
    qfFile.open(QIODevice::WriteOnly | QIODevice::Text);

    // text stream
    QTextStream qtsTextStream(&qfFile);
    qtsTextStream.setCodec(_pewWidget->GetCodec().toLocal8Bit());

    // process export
    _pewWidget->setUpdatesEnabled(false);
    _pewWidget->Refresh();

    // save result to file
    qtsTextStream << _pewWidget->GetText();
} // beginExport

const QString ExpPlaintext::getPluginName() const
{
	return tr("text file (txt)");
} // getPluginName

const void ExpPlaintext::on_pewWidget_ProgressExportSetMax(const int &pMax) const
{
    emit progressExportSetMax(pMax);
} // on_pewWidget_ProgressExportSetMax

const void ExpPlaintext::on_pewWidget_ProgressExportSetValue(const int &pValue) const
{
    emit progressExportSetValue(pValue);
} // on_pewWidget_ProgressExportSetValue

const void ExpPlaintext::on_pewWidget_VocabularyGetCategoryIds(ExpInterface::CategoryIdList *pCategoryIds) const
{
    emit vocabularyGetCategoryIds(pCategoryIds);
} // on_pewWidget_VocabularyGetCategoryIds

const void ExpPlaintext::on_pewWidget_VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const
{
    emit vocabularyGetCategoryName(pCategoryId, pName);
} // on_pewWidget_VocabularyGetCategoryName

const void ExpPlaintext::on_pewWidget_VocabularyGetMarks(QStringList *pMarks) const
{
    emit vocabularyGetMarks(pMarks);
} // on_pewWidget_VocabularyGetMarks

const void ExpPlaintext::on_pewWidget_VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const
{
    emit vocabularyGetMarkText(pRecordId, pMark, pText);
} // on_pewWidget_VocabularyGetMarkText

const void ExpPlaintext::on_pewWidget_VocabularyGetRecordCount(const int &pCategoryId, quint32 *pCount) const
{
    emit vocabularyGetRecordCount(pCategoryId, pCount);
} // on_pewWidget_VocabularyGetRecordCount

const void ExpPlaintext::on_pewWidget_VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::RecordIdList *pRecordIds) const
{
    emit vocabularyGetRecordIds(pCategoryId, pRecordIds);
} // on_pewWidget_VocabularyGetRecordCount

const void ExpPlaintext::setupUi(QWidget *pParent)
{
	_pewWidget = new PlaintextExportWidget(pParent);
	QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
	pLayout->insertWidget(WIDGET_POSITION, _pewWidget);

    connect(_pewWidget, SIGNAL(ProgressExportSetMax(const int &)), SLOT(on_pewWidget_ProgressExportSetMax(const int &)));
    connect(_pewWidget, SIGNAL(ProgressExportSetValue(const int &)), SLOT(on_pewWidget_ProgressExportSetValue(const int &)));
    connect(_pewWidget, SIGNAL(VocabularyGetCategoryIds(ExpInterface::CategoryIdList *)), SLOT(on_pewWidget_VocabularyGetCategoryIds(ExpInterface::CategoryIdList *)));
    connect(_pewWidget, SIGNAL(VocabularyGetCategoryName(const int &, QString *)), SLOT(on_pewWidget_VocabularyGetCategoryName(const int &, QString *)));
    connect(_pewWidget, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_pewWidget_VocabularyGetMarks(QStringList *)));
    connect(_pewWidget, SIGNAL(VocabularyGetMarkText(const int &, const QString &, QString *)), SLOT(on_pewWidget_VocabularyGetMarkText(const int &, const QString &, QString *)));
    connect(_pewWidget, SIGNAL(VocabularyGetRecordCount(const int &, quint32 *)), SLOT(on_pewWidget_VocabularyGetRecordCount(const int &, quint32 *)));
    connect(_pewWidget, SIGNAL(VocabularyGetRecordIds(const int &, ExpInterface::RecordIdList *)), SLOT(on_pewWidget_VocabularyGetRecordIds(const int &, ExpInterface::RecordIdList *)));
} // setupUi