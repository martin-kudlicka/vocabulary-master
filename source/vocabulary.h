#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "vocabulary/vocabularydatabase.h"
#include <QtCore/QMap>

class Vocabulary : public VocabularyDatabase
{
	public:
        Vocabulary();

		const int addCategory(const QString &name);
#ifndef EDITION_FREE
        const void addField();
#endif
		const void addRecord(quint8 categoryId);
#ifndef EDITION_FREE
		const void addRecord(quint8 categoryId, const QStringList &data);
#endif
        const void beginEdit();
		const void close();
        const void endEdit(bool save = true);
		const bool fieldHasAttribute(quint8 fieldId, FieldAttribute attribute) const;
        const CategoryIdList categoryIds() const;
		const QString dataText(quint8 categoryId, quint32 row, quint8 fieldId) const;
		const QString dataText(quint32 recordId, quint8 fieldId) const;
		const FieldAttributes fieldAttributes(quint8 fieldId) const;
#ifndef EDITION_FREE
		const FieldBuiltIn fieldBuiltIn(quint8 fieldId) const;
#endif
        const int fieldCount() const;
		const int fieldId(quint8 position) const;
        const FieldIdList fieldIds() const;
		const FieldLanguage fieldLanguage(quint8 fieldId) const;
		const QString fieldName(quint8 fieldId) const;
		const QString fieldTemplateName(quint8 fieldId) const;
		const FieldType fieldType(quint8 fieldId) const;
#ifndef EDITION_FREE
        const QStringList record(quint32 recordId) const;
#endif
		const int recordCategory(quint32 recordId) const;
		const int recordCount() const;
		const int recordCount(quint8 categoryId) const;
#ifndef EDITION_FREE
		const int recordCount(quint8 categoryId, bool enabled) const;
		const int recordCount(bool enabled) const;
		const bool recordEnabled(quint32 recordId) const;
#endif
        const int recordId(quint32 row) const;
        const int recordId(quint8 categoryId, quint32 row) const;
        const RecordIdList recordIds(quint8 categoryId) const;
		const void new2(
#ifndef EDITION_TRY
			const QString &filePath
#endif
			);
#ifndef EDITION_TRY
		const void open(const QString &filePath);
#else
        const void openMemory();
#endif
		const void removeCategory(quint8 categoryId);
#ifndef EDITION_FREE
        const void removeField(quint8 fieldId);
#endif
		const void removeRecord(quint8 categoryId, quint32 row);
		const void setDataText(quint8 categoryId, quint32 row, quint8 fieldId, const QString &data);
		const void setDataText(quint32 recordId, quint8 fieldId, const QString &data);
#ifndef EDITION_FREE
		const void setFieldAttributes(quint8 fieldId, FieldAttributes attributes);
		const void setFieldLanguage(quint8 fieldId, FieldLanguage language);
		const void setFieldName(quint8 fieldId, const QString &name);
		const void setFieldTemplateName(quint8 fieldId, const QString &templateName);
		const void setRecordByRowCategory(quint8 oldCategoryId, quint32 recordRow, quint8 newCategoryId);
		const void swapFields(quint8 sourceId, quint8 destinationId);
#endif

	private:
        struct FieldData {
            QString templateName;
            QString name;
            FieldType type;
            FieldAttributes attributes;
#ifndef EDITION_FREE
            FieldBuiltIn builtIn;
#endif
            FieldLanguage language;
        }; // FieldData

        typedef QHash<int, RecordIdList> CategoryRecordsMap;
        typedef QMap<int, FieldData> FieldDataMap;

		CategoryRecordsMap _tcrmCategoryRecords;
        FieldDataMap _tfdmFieldData;
        FieldDataMap _tfdmFieldDataBackup;
        RecordDataHash *_trdhRecordData;
        RecordDataHash _trdhRecordDataBackup;

		const void clearCache();
		const FieldData getFieldData(quint8 fieldId) const;
		const void initCache();
}; // Vocabulary

#endif // VOCABULARY_H