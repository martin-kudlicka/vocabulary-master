#ifndef IMPANKI_H
#define IMPANKI_H

#include "../common/imp-interface.h"

class AnkiImportWidget;

class ImpAnki : public QObject, private ImpInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IID_IMPINTERFACE)
  Q_INTERFACES(ImpInterface)

  public:
    ImpAnki();

  private:
    enum class ColumnPosition
    {
      N1
    };
    enum class FieldNum
    {
      N1
    };

    AnkiImportWidget *_widget;
    QSqlDatabase      _database;

    virtual ~ImpAnki() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual void        close      ()                                       Q_DECL_OVERRIDE;
    virtual QString     filter     () const                                 Q_DECL_OVERRIDE;
    virtual QStringList marks      () const                                 Q_DECL_OVERRIDE;
    virtual bool        open       (const QString &fileName)                Q_DECL_OVERRIDE;
    virtual quintptr    recordCount() const                                 Q_DECL_OVERRIDE;
    virtual QString     recordData (quintptr recordId, const QString &mark) Q_DECL_OVERRIDE;
    virtual void        setupUI    (QGroupBox *parent)                      Q_DECL_OVERRIDE;
};

#endif