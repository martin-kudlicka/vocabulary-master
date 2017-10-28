#ifndef EXPINTERFACE_H
#define EXPINTERFACE_H

#include "translationhandler.h"

#define IID_EXPINTERFACE "cz.martink.VocabularyMaster.ExpInterface"

class ExpInterface : public QObject, public TranslationHandler
{
  Q_OBJECT

  public:
    using CategoryIdList = QList<quintptr>;
    using RecordIdList   = QList<quintptr> ;

    static const auto WIDGET_POSITION = 0;

    virtual void    beginExport() const          = 0;
    virtual QString pluginName () const          = 0;
    virtual void    setupUi    (QWidget *parent) = 0;

  protected:
    virtual ~ExpInterface() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  Q_SIGNALS:
    void progressExportSetMax     (quintptr max)                                               const;
    void progressExportSetValue   (quintptr value)                                             const;
    void vocabularyGetCategoryIds (ExpInterface::CategoryIdList *categoryIds)                  const;
    void vocabularyGetCategoryName(quintptr categoryId, QString *name)                         const;
    void vocabularyGetMarks       (QStringList *marks)                                         const;
    void vocabularyGetMarkText    (quintptr recordId, const QString &mark, QString *text)      const;
    void vocabularyGetRecordCount (quintptr categoryId, quintptr *count)                       const;
    void vocabularyGetRecordIds   (quintptr categoryId, ExpInterface::RecordIdList *recordIds) const;
};

Q_DECLARE_INTERFACE(ExpInterface, IID_EXPINTERFACE);

#endif