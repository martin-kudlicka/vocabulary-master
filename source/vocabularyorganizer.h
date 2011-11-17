#pragma once

#include "vocabulary.h"
#include "settings.h"

class VocabularyOrganizer : public QObject
{
	Q_OBJECT

	public:
		struct sRecordInfo {
			Vocabulary *vVocabulary;
			int iId;
		}; // sRecordInfo

		VocabularyOrganizer(Settings *pSettings);

		const void Close(const int &pIndex);
		const int GetRecordCount() const;
		const int GetRecordCount(const bool &pEnabled) const;
		const sRecordInfo GetRecordInfo(const int &pRow) const;
		const int GetVocabularyCount() const;
		const Vocabulary *GetVocabularyInfo(const int &pIndex) const;
		const bool IsOpen() const;
		const void New(
#ifndef TRY
			const QString &pFile
#endif
			);
#ifndef TRY
		const void Open(const QString &pFile, QWidget *pParent);
		const void OpenAll(QWidget *pParent);
#endif

	private:
		QList<Vocabulary *> _qlVocabularies;
		Settings *_sSettings;

	signals:
		void VocabularyClose(const Vocabulary *pVocabulary) const;
}; // VocabularyOrganizer