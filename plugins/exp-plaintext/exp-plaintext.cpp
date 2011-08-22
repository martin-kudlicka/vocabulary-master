#include "exp-plaintext.h"

#include "plaintextexportwidget.h"

const QString ExpPlaintext::GetPluginName() const
{
	return tr("plaintext (txt)");
} // GetPluginName

void ExpPlaintext::on_pewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const
{
    emit VocabularyGetCategoryIds(pCategoryIds);
} // on_pewWidget_VocabularyGetCategoryIds

void ExpPlaintext::on_pewWidget_VocabularyGetMarks(QStringList *pMarks) const
{
    emit VocabularyGetMarks(pMarks);
} // on_pewWidget_VocabularyGetMarks

const void ExpPlaintext::SetupUI(QWidget *pParent)
{
	PlaintextExportWidget *pewWidget = new PlaintextExportWidget(pParent);
	QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
	pLayout->insertWidget(WIDGET_POSITION, pewWidget);

    connect(pewWidget, SIGNAL(VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)), SLOT(on_pewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *)));
    connect(pewWidget, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_pewWidget_VocabularyGetMarks(QStringList *)));
} // SetupUI

Q_EXPORT_PLUGIN2(exp-plaintext, ExpPlaintext)