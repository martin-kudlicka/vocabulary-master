#ifndef EXPINTERFACE_H
#define EXPINTERFACE_H

#include "translationhandler.h"

#define IID_EXPINTERFACE "cz.martinkudlicka.VocabularyMaster.ExpInterface"

class ExpInterface : public QObject, public TranslationHandler
{
  Q_OBJECT

  public:
    typedef QList<quintptr> CategoryIdList;
    typedef QList<quintptr> RecordIdList;

    static const auto WIDGET_POSITION = 0;

    virtual void    beginExport() const          = 0;
    virtual QString pluginName () const          = 0;
    virtual void    setupUi    (QWidget *parent) = 0;

  protected:
    virtual ~ExpInterface() override;

  signals:
    void progressExportSetMax     (quintptr pMax)                                                const;
    void progressExportSetValue   (quintptr pValue)                                              const;
    void vocabularyGetCategoryIds (ExpInterface::CategoryIdList *pCategoryIds)                   const;
    void vocabularyGetCategoryName(quintptr pCategoryId, QString *pName)                         const;
    void vocabularyGetMarks       (QStringList *pMarks)                                          const;
    void vocabularyGetMarkText    (quintptr pRecordId, const QString &pMark, QString *pText)     const;
    void vocabularyGetRecordCount (quintptr pCategoryId, quintptr *pCount)                       const;
    void vocabularyGetRecordIds   (quintptr pCategoryId, ExpInterface::RecordIdList *pRecordIds) const;
};

Q_DECLARE_INTERFACE(ExpInterface, IID_EXPINTERFACE);

#endif