#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "vocabulary/vocabularydatabase.h"

class Vocabulary : public VocabularyDatabase
{
	public:
        Vocabulary();

		const int AddCategory(const QString &pName);
		const void AddRecord(const int &pCategoryId);
#ifndef FREE
		const void AddRecord(const int &pCategoryId, const QStringList &pData);
#endif
        const QString GetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const;
        const QString GetDataText(const int &pRecordId, const int &pFieldId) const;
		const int GetRecordCount() const;
		const int GetRecordCount(const int &pCategoryId) const;
#ifndef FREE
		const int GetRecordCount(const bool &pEnabled) const;
#endif
        const tRecordIdList GetRecordIds(const int &pCategoryId) const;
		const void New(const QString &pFilePath);
#ifndef TRY
		const void Open(const QString &pFilePath);
#else
        const void OpenMemory();
#endif
		const void RemoveCategory(const int &pCategoryId);
#ifndef FREE
        const void RemoveField(const int &pFieldId);
#endif
		const void RemoveRecord(const int &pCategoryId, const int &pRow);
        const void SetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId, const QString &pData);

	private:
		QHash<int, tRecordIdList> _qhCategoryRecords;
        tRecordDataHash *_trdhRecordData;

		const void ClearCache();
		const void InitCache();
        const void SetDataText(const int &pRecordId, const int &pFieldId, const QString &pData);
}; // VocabularyDatabase

#endif // VOCABULARY_H