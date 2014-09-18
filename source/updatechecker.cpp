#include "updatechecker.h"

UpdateChecker::UpdateChecker(const Settings *settings, QObject *parent /* NULL */) : QObject(parent), _settings(settings)
{
	_lastReply = NULL;

	_currentVersion.usMajor    = 1;
	_currentVersion.usMinor    = 3;
	_currentVersion.usMinor2   = 0;
	_currentVersion.usRevision = 654;

	connect(&_networkAccessManager, SIGNAL(finished(QNetworkReply *)), SLOT(on_networkAccessManager_finished(QNetworkReply *)));
} // UpdateChecker

UpdateChecker::~UpdateChecker()
{
	if (_lastReply)
	{
		_lastReply->deleteLater();
	} // if
} // ~UpdateChecker

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
	} // if else

	// check
	_networkAccessManager.get(QNetworkRequest(QUrl("http://vocabulary-master.cz/sites/default/files/releases/version.txt")));
} // checkForUpdate

QNetworkReply::NetworkError UpdateChecker::checkResult() const
{
	return _lastReply->error();
} // checkResult

QString UpdateChecker::currentVersion() const
{
	return QString("%1.%2.%3.%4").arg(_currentVersion.usMajor).arg(_currentVersion.usMinor).arg(_currentVersion.usMinor2).arg(_currentVersion.usRevision);
} // currentVersion

bool UpdateChecker::updateAvailable() const
{
	if (_updateVersion.usMajor >= _currentVersion.usMajor)
	{
		if (_updateVersion.usMajor > _currentVersion.usMajor)
		{
			return true;
		}
		else
		{
			if (_updateVersion.usMinor >= _currentVersion.usMinor)
			{
				if (_updateVersion.usMinor > _currentVersion.usMinor)
				{
					return true;
				}
				else
				{
					if (_updateVersion.usMinor2 >= _currentVersion.usMinor2)
					{
						if (_updateVersion.usMinor2 > _currentVersion.usMinor2)
						{
							return true;
						}
						else
						{
							if (_updateVersion.usRevision > _currentVersion.usRevision)
							{
								return true;
							} // if
						} // if else
					} // if
				} // if else
			} // if
		} // if else
	} // if

	return false;
} // updateAvailable

QString UpdateChecker::updateVersion() const
{
	return QString("%1.%2.%3.%4").arg(_updateVersion.usMajor).arg(_updateVersion.usMinor).arg(_updateVersion.usMinor2).arg(_updateVersion.usRevision);
} // updateVersion

void UpdateChecker::analyzeReply()
{
	const QByteArray version        = _lastReply->readAll();
	const QList<QByteArray> verInfo = version.split('.');

	_updateVersion.usMajor    = verInfo.at(VerInfoMajor).toUShort();
	_updateVersion.usMinor    = verInfo.at(VerInfoMinor).toUShort();
	_updateVersion.usMinor2   = verInfo.at(VerInfoMinor2).toUShort();
	_updateVersion.usRevision = verInfo.at(VerInfoRevision).toUShort();
} // analyzeReply

void UpdateChecker::on_networkAccessManager_finished(QNetworkReply *reply)
{
	if (_lastReply) 
	{
		_lastReply->deleteLater();
	} // if

	_lastReply = reply;
	if (reply->error() == QNetworkReply::NoError)
	{
		analyzeReply();
	} // if

	emit finished();
} // on_networkAccessManager_finished