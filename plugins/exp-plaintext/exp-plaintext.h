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
        void on_pewWidget_VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const;
        void on_pewWidget_VocabularyGetMarks(QStringList *pMarks) const;
}; // ExpPlaintext

#endif // EXP_PLAINTEXT_H
