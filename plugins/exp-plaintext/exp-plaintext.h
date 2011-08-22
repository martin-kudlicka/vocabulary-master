#ifndef EXP_PLAINTEXT_H
#define EXP_PLAINTEXT_H

#include "../common/exp-interface.h"

class ExpPlaintext : public ExpInterface
{
	Q_OBJECT
	Q_INTERFACES(ExpInterface)

	private:
		virtual const QString GetPluginName() const;
		virtual const void SetupUI(QWidget *pParent);

    private slots:
        const void on_pewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const;
        const void on_pewWidget_VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const;
        const void on_pewWidget_VocabularyGetMarks(QStringList *pMarks) const;
        const void on_pewWidget_VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const;
        const void on_pewWidget_VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const;
}; // ExpPlaintext

#endif // EXP_PLAINTEXT_H
