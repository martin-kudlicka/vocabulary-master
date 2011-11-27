#include "updatechecker.h"

UpdateChecker::~UpdateChecker()
{
	if (_qnrLastReply) {
		_qnrLastReply->deleteLater();
	} // if
} // ~UpdateChecker

const void UpdateChecker::AnalyzeReply()
{
	QByteArray qbaVersion = _qnrLastReply->readAll();
	QList<QByteArray> qlVerInfo = qbaVersion.split('.');

	_svUpdate.usMajor = qlVerInfo.at(VerInfoMajor).toUShort();
	_svUpdate.usMinor = qlVerInfo.at(VerInfoMinor).toUShort();
	_svUpdate.usMinor2 = qlVerInfo.at(VerInfoMinor2).toUShort();
	_svUpdate.usRevision = qlVerInfo.at(VerInfoRevision).toUShort();
} // AnalyzeReply

const void UpdateChecker::CheckForUpdate()
{
	// proxy settings
	if (_sSettings->GetUseProxy()) {
		QNetworkProxy qnpProxy(_sSettings->GetProxyType(), _sSettings->GetProxyHostname(), _sSettings->GetProxyPort(), _sSettings->GetProxyUsername(), _sSettings->GetProxyPassword());
		_qnamNetworkManager.setProxy(qnpProxy);
	} else {
		_qnamNetworkManager.setProxy(QNetworkProxy());
	} // if else

	// check
	_qnamNetworkManager.get(QNetworkRequest(QUrl("http://vocabulary-master.cz/sites/default/files/releases/version.txt")));
} // CheckForUpdate

const QNetworkReply::NetworkError UpdateChecker::GetCheckResult() const
{
	return _qnrLastReply->error();
} // GetCheckResult

const QString UpdateChecker::GetCurrentVersion() const
{
	return QString("%1.%2.%3.%4").arg(_svCurrent.usMajor).arg(_svCurrent.usMinor).arg(_svCurrent.usMinor2).arg(_svCurrent.usRevision);
} // GetCurrentVersion

const QString UpdateChecker::GetUpdateVersion() const
{
	return QString("%1.%2.%3.%4").arg(_svUpdate.usMajor).arg(_svUpdate.usMinor).arg(_svUpdate.usMinor2).arg(_svUpdate.usRevision);
} // GetUpdateVersion

const bool UpdateChecker::IsUpdateAvailable() const
{
	if (_svUpdate.usMajor >= _svCurrent.usMajor) {
		if (_svUpdate.usMajor > _svCurrent.usMajor) {
			return true;
		} else {
			if (_svUpdate.usMinor >= _svCurrent.usMinor) {
				if (_svUpdate.usMinor > _svCurrent.usMinor) {
					return true;
				} else {
					if (_svUpdate.usMinor2 >= _svCurrent.usMinor2) {
						if (_svUpdate.usMinor2 > _svCurrent.usMinor2) {
							return true;
						} else {
							if (_svUpdate.usRevision > _svCurrent.usRevision) {
								return true;
							} // if
						} // if else
					} // if
				} // if else
			} // if
		} // if else
	} // if

	return false;
} // IsUpdateAvailable

void UpdateChecker::on_qnamNetworkManager_finished(QNetworkReply *reply)
{
	if (_qnrLastReply) {
		_qnrLastReply->deleteLater();
	} // if

	_qnrLastReply = reply;
	if (reply->error() == QNetworkReply::NoError) {
		AnalyzeReply();
	} // if

	emit Finished();
} // on_qnamNetworkManager_finished

UpdateChecker::UpdateChecker(const Settings *pSettings, QObject *pParent /* NULL */) : QObject(pParent)
{
	_sSettings = pSettings;

	_qnrLastReply = NULL;

	_svCurrent.usMajor = 1;
	_svCurrent.usMinor = 3;
	_svCurrent.usMinor2 = 0;
	_svCurrent.usRevision = 654;

	connect(&_qnamNetworkManager, SIGNAL(finished(QNetworkReply *)), SLOT(on_qnamNetworkManager_finished(QNetworkReply *)));
} // UpdateChecker