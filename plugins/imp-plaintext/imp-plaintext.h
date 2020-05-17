#ifndef IMPPLAINTEXT_H
#define IMPPLAINTEXT_H

#include "../common/imp-interface.h"
#include "plaintextimportwidget/plaintextfile.h"

class PlaintextImportWidget;

class ImpPlaintext : public QObject, private ImpInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IID_IMPINTERFACE)
  Q_INTERFACES(ImpInterface)

  public:
    ImpPlaintext();

  private:
    quintptr               _cachedRecord;
    PlaintextFile          _plaintextFile;
    PlaintextImportWidget *_widget;
    QStringList            _cachedCapture;

    virtual ~ImpPlaintext() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual void        close      ()                                     Q_DECL_OVERRIDE;
    virtual QString     filter     () const                               Q_DECL_OVERRIDE;
    virtual QStringList marks      () const                               Q_DECL_OVERRIDE;
    virtual bool        open       (const QString &fileName)              Q_DECL_OVERRIDE;
    virtual quintptr    recordCount() const                               Q_DECL_OVERRIDE;
    virtual QString     recordData (quintptr record, const QString &mark) Q_DECL_OVERRIDE;
    virtual void        setupUI    (QGroupBox *parent)                    Q_DECL_OVERRIDE;
};

#endif