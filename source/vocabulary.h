#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "vocabulary/vocabularydatabase.h"
#include "settings.h"

class Vocabulary : public VocabularyDatabase
{
	public:
                 Vocabulary(const Settings *settings);
		virtual ~Vocabulary();

		quint8          addCategory           (const QString &name);
#ifndef EDITION_FREE
		void            addField              ();
#endif
		void            addRecord             (quint8 categoryId);
#ifndef EDITION_FREE
		void            addRecord             (quint8 categoryId, const QStringList &data);
#endif
		void            beginEdit             ();
		void            close                 ();
		void            endEdit               (bool save = true);
		bool            fieldHasAttribute     (quint8 fieldId, FieldAttribute attribute)       const;
        CategoryIdList  categoryIds           ()                                               const;
		QString         dataText              (quint8 categoryId, quint32 row, quint8 fieldId) const;
		QString         dataText              (quint32 recordId, quint8 fieldId)               const;
		FieldAttributes fieldAttributes       (quint8 fieldId)                                 const;
#ifndef EDITION_FREE
		FieldBuiltIn    fieldBuiltIn          (quint8 fieldId)                                 const;
#endif
        quint8          fieldCount            ()                                               const;
		quint8          fieldId               (quint8 position)                                const;
		FieldIdList     fieldIds              ()                                               const;
		FieldLanguage   fieldLanguage         (quint8 fieldId)                                 const;
		QString         fieldName             (quint8 fieldId)                                 const;
		QString         fieldTemplateName     (quint8 fieldId)                                 const;
		FieldType       fieldType             (quint8 fieldId)                                 const;
#ifndef EDITION_FREE
		QStringList     record                (quint32 recordId)                               const;
#endif
		quint8          recordCategory        (quint32 recordId)                               const;
		quint32         recordCount           ()                                               const;
		quint32         recordCount           (quint8 categoryId)                              const;
#ifndef EDITION_FREE
		quint32         recordCount           (quint8 categoryId, bool enabled)                const;
		quint32         recordCount           (bool enabled)                                   const;
		bool            recordEnabled         (quint32 recordId)                               const;
#endif
		quint32         recordId              (quint32 row)                                    const;
		quint32         recordId              (quint8 categoryId, quint32 row)                 const;
		RecordIdList    recordIds             (quint8 categoryId)                              const;
		void            new2                  (
#ifndef EDITION_TRY
			const QString &filePath
#endif
			);
#ifndef EDITION_TRY
		void            open                  (const QString &filePath);
#else
		void            openMemory            ();
#endif
		void            removeCategory        (quint8 categoryId);
#ifndef EDITION_FREE
        void            removeField           (quint8 fieldId);
#endif
		void            removeRecord          (quint8 categoryId, quint32 row);
		void            setDataText           (quint8 categoryId, quint32 row, quint8 fieldId, const QString &data);
		void            setDataText           (quint32 recordId, quint8 fieldId, const QString &data);
#ifndef EDITION_FREE
		void            setFieldAttributes    (quint8 fieldId, FieldAttributes attributes);
		void            setFieldLanguage      (quint8 fieldId, FieldLanguage language);
		void            setFieldName          (quint8 fieldId, const QString &name);
		void            setFieldTemplateName  (quint8 fieldId, const QString &templateName);
		void            setRecordByRowCategory(quint8 oldCategoryId, quint32 recordRow, quint8 newCategoryId);
		void            swapFields            (quint8 sourceId, quint8 destinationId);
#endif

	private:
        struct FieldData {
            QString         templateName;
            QString         name;
            FieldType       type;
            FieldAttributes attributes;
#ifndef EDITION_FREE
            FieldBuiltIn    builtIn;
#endif
            FieldLanguage   language;
        }; // FieldData

        typedef QHash<quint8, RecordIdList> CategoryRecordsHash;
		typedef QHash<quint8, FieldData>    FieldDataHash;

		      bool                _cacheEnabled;
		      CategoryRecordsHash _categoryRecords;
              FieldDataHash       _fieldData;
              FieldDataHash       _fieldDataBackup;
              RecordDataHash     *_recordData;
              RecordDataHash      _recordDataBackup;
		const Settings           *_settings;

		void      clearCache  ();
		FieldData getFieldData(quint8 fieldId) const;
		void      initCache   ();
}; // Vocabulary

#endif // VOCABULARY_H