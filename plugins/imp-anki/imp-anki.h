#ifndef IMPANKI_H
#define IMPANKI_H

#include "../common/imp-interface.h"
#include <QtSql/QSqlDatabase>

class AnkiImportWidget;

class ImpAnki : public QObject, private ImpInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IID_IMPINTERFACE FILE "imp-anki.json")
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

    virtual ~ImpAnki() override;

    virtual void        close      ()                                       override;
    virtual QString     filter     () const                                 override;
    virtual QStringList marks      () const                                 override;
    virtual bool        open       (const QString &fileName)                override;
    virtual quintptr    recordCount() const                                 override;
    virtual QString     recordData (quintptr recordId, const QString &mark) override;
    virtual void        setupUI    (QGroupBox *parent)                      override;
};

#endif