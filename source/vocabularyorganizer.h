#pragma once

#include "vocabulary.h"
#include "settings.h"

class VocabularyOrganizer : public QObject
{
	Q_OBJECT

	public:
		struct RecordInfo {
			Vocabulary *vVocabulary;
			int iId;
		}; // RecordInfo
		struct VocabularyInfo {
			Settings::VocabularyInfo sviVocabularyInfo;
			Vocabulary *vVocabulary;
		}; // VocabularyInfo

		VocabularyOrganizer(Settings *pSettings);

#ifndef EDITION_TRY
		const void AddExisting(VocabularyInfo &pVocabulary, QWidget *pParent);
#endif
		const void AddNew(
#ifndef EDITION_TRY
			const QString &pFile
#endif
			);
		const int GetRecordCount() const;
		const int GetRecordCount(const bool &pEnabled) const;
		const RecordInfo GetRecordInfo(const int &pRow) const;
		const int GetVocabularyCount() const;
		const VocabularyInfo &GetVocabularyInfo(const int &pIndex) const;
		const bool IsOpen() const;
#ifndef EDITION_TRY
		const void OpenAll(QWidget *pParent);
#endif
		const void Remove(const int &pIndex);
#ifndef EDITION_TRY
		const void SaveAll();
# ifndef EDITION_FREE
		const void SetVocabularyEnabled(const int &pIndex, const bool &pEnabled, QWidget *pParent);
# endif
#endif

	private:
		QList<VocabularyInfo> _qlVocabularies;
		Settings *_sSettings;

#ifndef EDITION_TRY
		const void Open(VocabularyInfo *pVocabulary, QWidget *pParent);
#endif

	signals:
		void VocabularyClose(const Vocabulary *pVocabulary) const;
}; // VocabularyOrganizer