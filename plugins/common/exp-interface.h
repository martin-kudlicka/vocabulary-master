#ifndef EXPINTERFACE_H
#define EXPINTERFACE_H

#include <QtPlugin>

static const char *EXP_INTERFACE = "Exp Interface 1.0";

class ExpInterface : public QObject
{
    Q_OBJECT

	public:
        typedef QList<int> tCategoryIdList;
        typedef QList<int> tRecordIdList;

		static const int WIDGET_POSITION = 0;

		virtual const QString GetPluginName() const = 0;
		virtual const void SetupUI(QWidget *pParent) = 0;

    signals:
        void VocabularyGetCategoryIds(ExpInterface::tCategoryIdList *pCategoryIds) const;
        void VocabularyGetCategoryName(const int &pCategoryId, QString *pName) const;
        void VocabularyGetMarks(QStringList *pMarks) const;
        void VocabularyGetMarkText(const int &pRecordId, const QString &pMark, QString *pText) const;
        void VocabularyGetRecordIds(const int &pCategoryId, ExpInterface::tRecordIdList *pRecordIds) const;
}; // ExpInterface

Q_DECLARE_INTERFACE(ExpInterface, EXP_INTERFACE);

#endif // EXPINTERFACE_H