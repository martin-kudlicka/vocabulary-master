#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include "settings.h"
#include <QtNetwork/QNetworkReply>

class UpdateChecker : public QObject
{
	Q_OBJECT

	public:
		~UpdateChecker();
		UpdateChecker(const Settings *pSettings, QObject *pParent = NULL);

		const void CheckForUpdate();
		const QNetworkReply::NetworkError GetCheckResult() const;
		const QString GetCurrentVersion() const;
		const QString GetUpdateVersion() const;
		const bool IsUpdateAvailable() const;

	private:
		enum eVerInfo {
			VerInfoMajor,
			VerInfoMinor,
			VerInfoMinor2,
			VerInfoRevision
		}; // eVerInfo

		struct sVersion {
			ushort usMajor;
			ushort usMinor;
			ushort usMinor2;
			ushort usRevision;
		}; // sVersion

		QNetworkAccessManager _qnamNetworkManager;
		QNetworkReply *_qnrLastReply;
		const Settings *_sSettings;
		sVersion _svCurrent;
		sVersion _svUpdate;

		const void AnalyzeReply();

	signals:
		void Finished() const;

	private slots:
		void on_qnamNetworkManager_finished(QNetworkReply *reply);
}; // UpdateChecker

#endif // UPDATECHECKER_H