#include "updatechecker.h"

#include "settings.h"

UpdateChecker::UpdateChecker(const Settings *settings, QObject *parent /* Q_NULLPTR */) : QObject(parent), _settings(settings), _lastReply(Q_NULLPTR)
{
  _currentVersion.major    = 1;
  _currentVersion.minor    = 3;
  _currentVersion.minor2   = 0;
  _currentVersion.revision = 654;

  connect(&_networkAccessManager, SIGNAL(finished(QNetworkReply *)), SLOT(on_networkAccessManager_finished(QNetworkReply *)));
}

UpdateChecker::~UpdateChecker()
{
  if (_lastReply)
  {
    _lastReply->deleteLater();
  }
}

void UpdateChecker::checkForUpdate()
{
  // proxy settings
  if (_settings->useProxy())
  {
    QNetworkProxy networkProxy(_settings->proxyType(), _settings->proxyHostname(), _settings->proxyPort(), _settings->proxyUsername(), _settings->proxyPassword());
    _networkAccessManager.setProxy(networkProxy);
  }
  else
  {
    _networkAccessManager.setProxy(QNetworkProxy());
  }

  // check
  _networkAccessManager.get(QNetworkRequest(QUrl("http://vocabulary-master.cz/sites/default/files/releases/version.txt")));
}

QNetworkReply::NetworkError UpdateChecker::checkResult() const
{
  return _lastReply->error();
}

QString UpdateChecker::currentVersion() const
{
  return QString("%1.%2.%3.%4").arg(_currentVersion.major).arg(_currentVersion.minor).arg(_currentVersion.minor2).arg(_currentVersion.revision);
}

bool UpdateChecker::updateAvailable() const
{
  if (_updateVersion.major >= _currentVersion.major)
  {
    if (_updateVersion.major > _currentVersion.major)
    {
      return true;
    }
    else
    {
      if (_updateVersion.minor >= _currentVersion.minor)
      {
        if (_updateVersion.minor > _currentVersion.minor)
        {
          return true;
        }
        else
        {
          if (_updateVersion.minor2 >= _currentVersion.minor2)
          {
            if (_updateVersion.minor2 > _currentVersion.minor2)
            {
              return true;
            }
            else
            {
              if (_updateVersion.revision > _currentVersion.revision)
              {
                return true;
              }
            }
          }
        }
      }
    }
  }

  return false;
}

QString UpdateChecker::updateVersion() const
{
  return QString("%1.%2.%3.%4").arg(_updateVersion.major).arg(_updateVersion.minor).arg(_updateVersion.minor2).arg(_updateVersion.revision);
}

void UpdateChecker::analyzeReply()
{
  const auto version = _lastReply->readAll();
  const auto verInfo = version.split('.');

  _updateVersion.major    = verInfo.at(static_cast<int>(VerInfo::Major)).toUInt();
  _updateVersion.minor    = verInfo.at(static_cast<int>(VerInfo::Minor)).toUInt();
  _updateVersion.minor2   = verInfo.at(static_cast<int>(VerInfo::Minor2)).toUInt();
  _updateVersion.revision = verInfo.at(static_cast<int>(VerInfo::Revision)).toUInt();
}

void UpdateChecker::on_networkAccessManager_finished(QNetworkReply *reply)
{
  if (_lastReply)
  {
    _lastReply->deleteLater();
  }

  _lastReply = reply;
  if (reply->error() == QNetworkReply::NoError)
  {
    analyzeReply();
  }

  emit finished();
}