#include "updatechecker.h"

#include "settings.h"
#include <QtCore/QCoreApplication>

UpdateChecker::UpdateChecker(const Settings *settings, QObject *parent /* Q_NULLPTR */) : QObject(parent), _settings(settings), _lastReply(Q_NULLPTR)
{
  _currentVersion = QVersionNumber::fromString(QCoreApplication::applicationVersion());

  connect(&_networkAccessManager, &QNetworkAccessManager::finished, this, &UpdateChecker::on_networkAccessManager_finished);
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
  _networkAccessManager.get(QNetworkRequest(QUrl("http://vocabulary-master.martink.cz/sites/vocabulary-master.martink.cz/files/releases/version.txt")));
}

QNetworkReply::NetworkError UpdateChecker::checkResult() const
{
  return _lastReply->error();
}

bool UpdateChecker::updateAvailable() const
{
  return _updateVersion > _currentVersion;
}

QString UpdateChecker::updateVersion() const
{
  return _updateVersion.toString();
}

void UpdateChecker::analyzeReply()
{
  const auto version = _lastReply->readAll();
  _updateVersion     = QVersionNumber::fromString(version);
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