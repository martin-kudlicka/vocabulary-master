#ifndef VOCABULARYDATABASE_H
#define VOCABULARYDATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtCore/QList>
#include <QtCore/QHash>
#ifdef EDITION_FREE
# include <QtCore/QObject>
#else
# include "../plugins/common/tts-interface.h"
#endif

const QString VARIABLE_MARK = "$";

class VocabularyDatabase : public QObject
{
	Q_OBJECT

    public:
        typedef QHash<quint8, QString>   DataHash;
        typedef QHash<quint32, DataHash> RecordDataHash;
        typedef QList<quintptr>          CategoryIdList;
		typedef QList<quintptr>            FieldIdList;
        typedef QList<quint8>            LanguageIdList;

        enum FieldAttribute
		{
            FieldAttributeNone,
#ifndef EDITION_FREE
            FieldAttributeSpeech,
#endif
            FieldAttributeShow    = 0x2,
            FieldAttributeBuiltIn = 0x4
        }; // FieldAttribute
        Q_DECLARE_FLAGS(FieldAttributes, FieldAttribute)
        enum FieldBuiltIn
		{
            FieldBuiltInNone,
            FieldBuiltInEnabled,
			FieldBuiltInPriority
        }; // FieldBuiltIn
		enum FieldLanguage
		{
			FieldLanguageUnknown,
			FieldLanguageLeft,
			FieldLanguageRight,
            FieldLanguageAll
		}; // FieldLanguage
		enum FieldType
		{
			FieldTypeUnknown,
			FieldTypeLineEdit,
			FieldTypeCheckBox,
			FieldTypeSpinBox
		}; // FieldType
#ifndef EDITION_FREE
		enum LanguageId
		{
			LanguageIdsNone,
			LanguageIdsUserDefined,
			LanguageIdsAllOnly,
			LanguageIdsAll = 0x4
		}; // LanguageIds
		Q_DECLARE_FLAGS(LanguageIds, LanguageId)
#endif

        static const qint8 NOT_FOUND = -1;

                 VocabularyDatabase(QObject *parent = nullptr);
		virtual ~VocabularyDatabase() override;

#ifndef EDITION_FREE
		quint8                  categoryCount              ()                                                  const;
		bool                    categoryEnabled            (quint8 categoryId)                                 const;
		quint8                  categoryId                 (quint8 row)                                        const;
#endif
		QString                 categoryName               (quint8 categoryId)                                 const;
#ifndef EDITION_FREE
		quint8                  categoryPriority           (quint8 categoryId)                                 const;
		bool                    isOpen                     ()                                                  const;
        LanguageIdList          languageIds                (LanguageIds type)                                  const;
#endif
		QString                 languageLearningTemplate   (quint8 languageId)                                 const;
		QString                 languageName               (quint8 languageId)                                 const;
#ifndef EDITION_FREE
		TTSInterface::TTSPlugin languageSpeech             (quint8 languageId)                                 const;
		QString                 languageTrayTemplate       (quint8 languageId)                                 const;
		QString                 languageVoice              (quint8 languageId)                                 const;
#endif
		QString                 name                       ()                                                  const;
		quint32                 row                        (quint32 recordId, quint8 categoryId)               const;
		quint32                 search                     (const QString &word, quint32 startId)              const;
#ifndef EDITION_FREE
		void                    setCategoryEnabled         (quint8 categoryId, bool enabled)                   const;
		void                    setCategoryPriority        (quint8 categoryId, quint8 priority)                const;
		void                    setLanguageLearningTemplate(quint8 languageId, const QString &templateText)    const;
#endif
		void                    setLanguageName            (quint8 languageId, const QString &name)            const;
#ifndef EDITION_FREE
		void                    setLanguageSpeech          (quint8 languageId, TTSInterface::TTSPlugin speech) const;
		void                    setLanguageTrayTemplate    (quint8 languageId, const QString &templateText)    const;
		void                    setLanguageVoice           (quint8 languageId, const QString &voice)           const;
#endif
		void                    setSettings                (const QString &key, const QString &value)          const;
		QString                 settings                   (const QString &key)                                const;
#ifndef EDITION_TRY
		const QString          &vocabularyFile             ()                                                  const;
#endif

    protected:
        typedef QList<quint32> RecordIdList;

		quint8          addCategory           (const QString &name)                                                 const;
#ifndef EDITION_FREE
		quint8          addField              ()                                                                    const;
#endif
		quint32         addRecord             (quint8 categoryId)                                                   const;
#ifndef EDITION_FREE
		quint32         addRecord             (quint8 categoryId, const QStringList &data)                          const;
#endif
        void            beginEdit             ();
		void            close                 ();
        void            endEdit               (bool save = true);
        CategoryIdList  categoryIds           ()                                                                    const;
        QString         dataText              (quint8 categoryId, quint32 row, quint8 fieldId)                      const;
        QString         dataText              (quint32 recordId, quint8 fieldId)                                    const;
        RecordDataHash *dataText              () const;
        FieldAttributes fieldAttributes       (quint8 fieldId)                                                      const;
#ifndef EDITION_FREE
		FieldBuiltIn    fieldBuiltIn          (quint8 fieldId)                                                      const;
		quint8          fieldCount            ()                                                                    const;
#endif
        quint8          fieldId               (quint8 position)                                                     const;
        FieldIdList     fieldIds              ()                                                                    const;
		FieldLanguage   fieldLanguage         (quint8 fieldId)                                                      const;
		QString         fieldName             (quint8 fieldId)                                                      const;
		QString         fieldTemplateName     (quint8 fieldId)                                                      const;
		FieldType       fieldType             (quint8 fieldId)                                                      const;
		void            new2                  (
#ifndef EDITION_TRY
			const QString &filePath
#endif
			);
        quint8          recordCategory        (quint32 recordId)                                                    const;
        quint32         recordCount           ()                                                                    const;
		quint32         recordCount           (quintptr categoryId)                                                   const;
#ifndef EDITION_FREE
		quint32         recordCount           (quint8 categoryId, bool enabled)                                     const;
#endif
		quint32         recordCount           (bool enabled)                                                        const;
        quint32         recordId              (quint32 row)                                                         const;
        quint32         recordId              (quint8 categoryId, quint32 row)                                      const;
        RecordIdList    recordIds             (quint8 categoryId)                                                   const;
#ifndef EDITION_TRY
        void            open                  (const QString &filePath);
#endif
        void            removeCategory        (quint8 categoryId)                                                   const;
#ifndef EDITION_FREE
		void            removeField           (quint8 fieldId)                                                      const;
#endif
		void            removeRecord          (quint8 categoryId, quint32 row)                                      const;
        void            setDataText           (quint8 categoryId, quint32 row, quint8 fieldId, const QString &data) const;
		void            setDataText           (quint32 recordId, quint8 fieldId, const QString &data)               const;
		void            setFieldAttributes    (quint8 fieldId, FieldAttributes attributes)                          const;
#ifndef EDITION_FREE
		void            setFieldLanguage      (quint8 fieldId, FieldLanguage language)                              const;
		void            setFieldName          (quint8 fieldId, const QString &name)                                 const;
		void            setFieldTemplateName  (quint8 fieldId, const QString &templateName)                         const;
		void            setRecordByRowCategory(quint8 oldCategoryId, quint32 recordRow, quint8 newCategoryId)       const;
		void            SetRecordCategory     (quint32 recordId, quint8 categoryId)                                 const;
		void            swapFields            (quint8 sourceId, quint8 destinationId)                               const;
#endif

    private:
		enum ColumnPosition
		{
			ColumnPosition1,
            ColumnPosition2,
            ColumnPosition3
		}; // ColumnPosition
        enum Version
		{
            Version1,
            Version2,
        }; // Version

		static const quint8 OPENPROGRESS_REFRESHINTERVAL = 100;
		static const quint8 PRIORITY_DEFAULT             = 1;

#ifdef EDITION_TRY
		static quint8 _memoryVocabularies;
		       quint8 _vocabularyNumber;
#endif
        QSqlDatabase _database;
#ifndef EDITION_TRY
        QString      _vocabularyFile;
#endif

		quint8       addField      (const QString &templateText, const QString &name, const FieldType &type, FieldAttributes attributes, FieldBuiltIn builtIn, FieldLanguage language) const;
		void         addLanguage   (const QString &name, const QString &learningTemplate
#ifndef EDITION_FREE
			, const QString &trayTemplate, TTSInterface::TTSPlugin ttsPlugin, const QString &voice
#endif
            )                                                                                                                                                                          const;
		void         closeDatabase ();
#ifndef EDITION_TRY
        RecordIdList recordIds     ()                                                                                                                                                  const;
#endif
		void         initialize    ()                                                                                                                                                  const;
		void         openDatabase  ();
		void         removeRecord  (quint32 recordId)                                                                                                                                  const;
#ifndef EDITION_FREE
		void         update        (const QString &table, quint8 columnId, const QSqlRecord &record)                                                                                   const;
#endif
#ifndef EDITION_TRY
        void         updateDatabase();
#endif

	signals:
		void setOpenProgressValue(quint8 value)        const;
		void setOpenProgressMax  (quint8 max)          const;
		void setVocabularyName   (const QString &name) const;
}; // VocabularyDatabase

Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::FieldAttributes)
#ifndef EDITION_FREE
Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::LanguageIds)
#endif

#endif // VOCABULARYDATABASE_H