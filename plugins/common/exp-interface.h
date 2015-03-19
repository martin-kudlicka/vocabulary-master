#ifndef EXPINTERFACE_H
#define EXPINTERFACE_H

#include <QtPlugin>
#include "translationhandler.h"

#define IID_EXPINTERFACE "cz.martinkudlicka.VocabularyMaster.ExpInterface"

class ExpInterface : public QObject, public TranslationHandler
{
    Q_OBJECT

	public:
        typedef QList<quint8>  CategoryIdList;
        typedef QList<quint32> RecordIdList;

		static const quint8 WIDGET_POSITION = 0;

        virtual void    beginExport() const      = 0;
		virtual QString pluginName()  const      = 0;
		virtual void    setupUi(QWidget *parent) = 0;

	protected:
		virtual ~ExpInterface() override;

    signals:
        void progressExportSetMax     (quint32 pMax)                                               const;
        void progressExportSetValue   (quint32 pValue)                                             const;
        void vocabularyGetCategoryIds (ExpInterface::CategoryIdList *pCategoryIds)                 const;
        void vocabularyGetCategoryName(quint8 pCategoryId, QString *pName)                         const;
        void vocabularyGetMarks       (QStringList *pMarks)                                        const;
        void vocabularyGetMarkText    (quint32 pRecordId, const QString &pMark, QString *pText)    const;
        void vocabularyGetRecordCount (quint8 pCategoryId, quint32 *pCount)                        const;
        void vocabularyGetRecordIds   (quint8 pCategoryId, ExpInterface::RecordIdList *pRecordIds) const;
}; // ExpInterface

Q_DECLARE_INTERFACE(ExpInterface, IID_EXPINTERFACE);

#endif // EXPINTERFACE_H