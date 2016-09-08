#ifndef LICENSE_H
#define LICENSE_H

#include <QtCore/QDate>
#include <QtCore/QUuid>

class Settings;

class License
{
  public:
    enum Status
    {
      None,
      Ok,
      Expired,
      Invalid
    };

    License(const Settings *settings);

    const QString &email()     const;
    const QString &firstName() const;
          bool     isLoaded()  const;
        //bool     isOk()      const;
    const QString &lastName()  const;
          void     refreshLicense();
    const Status  &status()    const;
    const QUuid   &uid()       const;
    const QDate   &validTo()   const;

  private:
    QDate           _validTo;
    QString         _email;
    QString         _firstName;
    QString         _lastName;
    QUuid           _identifier;
    const Settings *_settings;
    Status          _status;
};

#endif