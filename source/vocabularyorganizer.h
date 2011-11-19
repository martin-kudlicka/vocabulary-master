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
		struct sVocabulary {
			Settings::sVocabularyInfo sviVocabularyInfo;
			Vocabulary *vVocabulary;
		}; // sVocabulary

		VocabularyOrganizer(Settings *pSettings);

#ifndef TRY
		const void AddExisting(sVocabulary &pVocabulary, QWidget *pParent);
#endif
		const void AddNew(
#ifndef TRY
			const QString &pFile
#endif
			);
		const int GetRecordCount() const;
		const int GetRecordCount(const bool &pEnabled) const;
		const sRecordInfo GetRecordInfo(const int &pRow) const;
		const int GetVocabularyCount() const;
		const sVocabulary &GetVocabularyInfo(const int &pIndex) const;
		const bool IsOpen() const;
#ifndef TRY
		const void OpenAll(QWidget *pParent);
#endif
		const void Remove(const int &pIndex);
#ifndef TRY
		const void SaveAll();
#endif

	private:
		QList<sVocabulary> _qlVocabularies;
		Settings *_sSettings;

	signals:
		void VocabularyClose(const Vocabulary *pVocabulary) const;
}; // VocabularyOrganizer