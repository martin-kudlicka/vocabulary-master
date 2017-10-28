#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QVersionNumber>

class Settings;

class UpdateChecker : public QObject
{
  Q_OBJECT

  public:
             UpdateChecker(const Settings *settings);
    virtual ~UpdateChecker() Q_DECL_OVERRIDE;

    void                        checkForUpdate ();
    QNetworkReply::NetworkError checkResult    () const;
    bool                        updateAvailable() const;
    QString                     updateVersion  () const;

  private:
          QNetworkAccessManager _networkAccessManager;
          QNetworkReply        *_lastReply;
    const Settings             *_settings;
          QVersionNumber        _currentVersion;
          QVersionNumber        _updateVersion;

    void analyzeReply();

  Q_SIGNALS:
    void finished() const;

  private Q_SLOTS:
    void on_networkAccessManager_finished(QNetworkReply *reply);
};

#endif