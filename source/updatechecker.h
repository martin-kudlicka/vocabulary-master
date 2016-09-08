#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class Settings;

class UpdateChecker : public QObject
{
  Q_OBJECT

  public:
             UpdateChecker(const Settings *settings, QObject *parent = nullptr);
    virtual ~UpdateChecker() override;

    void                        checkForUpdate ();
    QNetworkReply::NetworkError checkResult    () const;
    QString                     currentVersion () const;
    bool                        updateAvailable() const;
    QString                     updateVersion  () const;

  private:
    enum class VerInfo
    {
      Major,
      Minor,
      Minor2,
      Revision
    };

    struct Version
    {
      quintptr major;
      quintptr minor;
      quintptr minor2;
      quintptr revision;
    };

          QNetworkAccessManager _networkAccessManager;
          QNetworkReply        *_lastReply;
    const Settings             *_settings;
          Version               _currentVersion;
          Version               _updateVersion;

    void analyzeReply();

  signals:
    void finished() const;

  private slots:
    void on_networkAccessManager_finished(QNetworkReply *reply);
};

#endif