#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "vocabulary/vocabularydatabase.h"
#include <QtCore/QMap>

class Vocabulary : public VocabularyDatabase
{
	public:
        Vocabulary();

		const int AddCategory(const QString &pName);
#ifndef FREE
        const void AddField();
#endif
		const void AddRecord(const int &pCategoryId);
#ifndef FREE
		const void AddRecord(const int &pCategoryId, const QStringList &pData);
#endif
        const void BeginEdit();
		const void Close();
        const void EndEdit(const bool &pSave = true);
		const bool FieldHasAttribute(const int &pFieldId, const eFieldAttribute &pAttribute) const;
        const tCategoryIdList GetCategoryIds() const;
        const QString GetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const;
        const QString GetDataText(const int &pRecordId, const int &pFieldId) const;
		const qfFieldAttributes GetFieldAttributes(const int &pFieldId) const;
#ifndef FREE
        const eFieldBuiltIn GetFieldBuiltIn(const int &pFieldId) const;
#endif
        const int GetFieldCount() const;
        const int GetFieldId(const int &pPosition) const;
        const tFieldIdList GetFieldIds() const;
        const eFieldLanguage GetFieldLanguage(const int &pFieldId) const;
        const QString GetFieldName(const int &pFieldId) const;
        const QString GetFieldTemplateName(const int &pFieldId) const;
		const eFieldType GetFieldType(const int &pFieldId) const;
#ifndef FREE
        const QStringList GetRecord(const int &pRecordId) const;
#endif
        const int GetRecordCategory(const int &pRecordId) const;
		const int GetRecordCount() const;
		const int GetRecordCount(const int &pCategoryId) const;
#ifndef FREE
		const int GetRecordCount(const int &pCategoryId, const bool &pEnabled) const;
		const int GetRecordCount(const bool &pEnabled) const;
		const bool GetRecordEnabled(const int &pRecordId) const;
#endif
        const int GetRecordId(const int &pRow) const;
        const int GetRecordId(const int &pCategoryId, const int &pRow) const;
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
		const void SetDataText(const int &pRecordId, const int &pFieldId, const QString &pData);
#ifndef FREE
		const void SetFieldAttributes(const int &pFieldId, const qfFieldAttributes &pAttributes);
        const void SetFieldLanguage(const int &pFieldId, const eFieldLanguage &pLanguage);
        const void SetFieldName(const int &pFieldId, const QString &pName);
        const void SetFieldTemplateName(const int &pFieldId, const QString &pTemplateName);
		const void SetRecordByRowCategory(const int &pOldCategoryId, const int &pRecordRow, const int &pNewCategoryId);
        const void SwapFields(const int &pSourceId, const int &pDestinationId);
#endif

	private:
        struct sFieldData {
            QString qsTemplateName;
            QString qsName;
            eFieldType eftType;
            qfFieldAttributes qfaAttributes;
#ifndef FREE
            eFieldBuiltIn efbBuiltIn;
#endif
            eFieldLanguage eflLanguage;
        }; // sFieldData

        typedef QHash<int, tRecordIdList> tCategoryRecordsMap;
        typedef QMap<int, sFieldData> tFieldDataMap;

		tCategoryRecordsMap _tcrmCategoryRecords;
        tFieldDataMap _tfdmFieldData;
        tFieldDataMap _tfdmFieldDataBackup;
        tRecordDataHash *_trdhRecordData;
        tRecordDataHash _trdhRecordDataBackup;

		const void ClearCache();
        const sFieldData GetFieldData(const int &pFieldId) const;
		const void InitCache();
}; // VocabularyDatabase

#endif // VOCABULARY_H