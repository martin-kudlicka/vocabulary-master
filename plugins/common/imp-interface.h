#ifndef IMPINTERFACE_H
#define IMPINTERFACE_H

#include "translationhandler.h"

class QGroupBox;

#define IID_IMPINTERFACE "cz.martink.VocabularyMaster.ImpInterface"

class ImpInterface : public TranslationHandler
{
  public:
    static const quintptr WIDGET_POSITION = 0;

    virtual void        close      ()                                       = 0;
    virtual QString     filter     () const                                 = 0;
    virtual QStringList marks      () const                                 = 0;
    virtual bool        open       (const QString &fileName)                = 0;
    virtual quintptr    recordCount() const                                 = 0;
    virtual QString     recordData (quintptr recordId, const QString &mark) = 0;
    virtual void        setupUI    (QGroupBox *parent)                      = 0;

  protected:
    virtual ~ImpInterface() Q_DECL_EQ_DEFAULT;
};

Q_DECLARE_INTERFACE(ImpInterface, IID_IMPINTERFACE);

#endif