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
        typedef QList<quint8>            CategoryIdList;
		typedef QList<quint8>            FieldIdList;
        typedef QList<quint8>            LanguageIdList;

        enum FieldAttribute {
            FieldAttributeNone,
#ifndef EDITION_FREE
            FieldAttributeSpeech,
#endif
            FieldAttributeShow    = 0x2,
            FieldAttributeBuiltIn = 0x4
        }; // FieldAttribute
        Q_DECLARE_FLAGS(FieldAttributes, FieldAttribute)
        enum FieldBuiltIn {
            FieldBuiltInNone,
            FieldBuiltInEnabled,
			FieldBuiltInPriority
        }; // FieldBuiltIn
		enum FieldLanguage {
			FieldLanguageUnknown,
			FieldLanguageLeft,
			FieldLanguageRight,
            FieldLanguageAll
		}; // FieldLanguage
		enum FieldType {
			FieldTypeUnknown,
			FieldTypeLineEdit,
			FieldTypeCheckBox,
			FieldTypeSpinBox
		}; // FieldType
#ifndef EDITION_FREE
		enum LanguageId {
			LanguageIdsNone,
			LanguageIdsUserDefined,
			LanguageIdsAllOnly,
			LanguageIdsAll = 0x4
		}; // LanguageIds
		Q_DECLARE_FLAGS(LanguageIds, LanguageId)
#endif

        static const qint8 NOT_FOUND = -1;

                 VocabularyDatabase(QObject *parent = NULL);
		virtual ~VocabularyDatabase();

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

        int addCategory(const QString &pName) const;
#ifndef EDITION_FREE
        int addField() const;
#endif
        int addRecord(const int &pCategoryId) const;
#ifndef EDITION_FREE
        int addRecord(const int &pCategoryId, const QStringList &pData) const;
#endif
        void beginEdit();
		void close();
        void endEdit(const bool &pSave = true);
        CategoryIdList categoryIds() const;
        /*QString dataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const;
        QString dataText(const int &pRecordId, const int &pFieldId) const;*/
        RecordDataHash *dataText() const;
        FieldAttributes fieldAttributes(const int &pFieldId) const;
#ifndef EDITION_FREE
        FieldBuiltIn fieldBuiltIn(const int &pFieldId) const;
        int fieldCount() const;
#endif
        //int GetFieldId(const int &pPosition) const;
        FieldIdList fieldIds() const;
        FieldLanguage fieldLanguage(const int &pFieldId) const;
        QString fieldName(const int &pFieldId) const;
        QString fieldTemplateName(const int &pFieldId) const;
        FieldType fieldType(const int &pFieldId) const;
        /*int GetRecordCategory(const int &pRecordId) const;
        int GetRecordCount() const;
		int GetRecordCount(const int &pCategoryId) const;
        int GetRecordCount(const bool &pEnabled) const;
        int GetRecordId(const int &pRow) const;
        int GetRecordId(const int &pCategoryId, const int &pRow) const;*/
        RecordIdList recordIds(const int &pCategoryId) const;
        void new2(
#ifndef EDITION_TRY
			const QString &pFilePath
#endif
			);
#ifndef EDITION_TRY
        void open(const QString &pFilePath);
#endif
        void removeCategory(const int &pCategoryId) const;
#ifndef EDITION_FREE
        void removeField(const int &pFieldId) const;
#endif
        void removeRecord(const int &pCategoryId, const int &pRow) const;
        //void setDataText(const int &pCategoryId, const int &pRow, const int &pFieldId, const QString &pData) const;
        void setDataText(const int &pRecordId, const int &pFieldId, const QString &pData) const;
        void setFieldAttributes(const int &pFieldId, const FieldAttributes &pAttributes) const;
#ifndef EDITION_FREE
        void setFieldLanguage(const int &pFieldId, const FieldLanguage &pLanguage) const;
        void setFieldName(const int &pFieldId, const QString &pName) const;
        void setFieldTemplateName(const int &pFieldId, const QString &pTemplateName) const;
		void SetRecordCategory(const int &pRecordId, const int &pCategoryId) const;
        void swapFields(const int &pSourceId, const int &pDestinationId) const;
#endif

    private:
		enum eColumnPosition {
			ColumnPosition1,
            ColumnPosition2,
            ColumnPosition3
		}; // eColumnPosition
        enum eVersion {
            Version1,
            Version2,
        }; // eVersion

		static const int OPENPROGRESS_REFRESHINTERVAL = 100;
		static const int PRIORITY_DEFAULT = 1;

#ifdef EDITION_TRY
		static int _iMemoryVocabularies;
		int _iVocabularyNumber;
#endif
        QSqlDatabase _qsdDatabase;
#ifndef EDITION_TRY
        QString _qsVocabularyFile;
#endif

        int addField(const QString &pTemplate, const QString &pName, const FieldType &pType, const FieldAttributes &pAttributes, const FieldBuiltIn &pBuiltIn, const FieldLanguage &pLanguage) const;
        void AddLanguage(const QString &pName, const QString &pLearningTemplate
#ifndef EDITION_FREE
            , const QString &pTrayTemplate, const TTSInterface::TTSPlugin &pTTSPlugin, const QString &pVoice
#endif
            ) const;
		void CloseDatabase();
#ifndef EDITION_TRY
        RecordIdList recordIds() const;
#endif
		void Initialize() const;
		void OpenDatabase();
		void removeRecord(const int &pRecordId) const;
#ifndef EDITION_FREE
		void Update(const QString &pTable, const int &pColumnId, const QSqlRecord &pRecord) const;
#endif
#ifndef EDITION_TRY
        void UpdateDatabase();
#endif

	signals:
		void SetOpenProgressValue(const int &pValue) const;
		void SetOpenProgressMax(const int &pMax) const;
		void SetVocabularyName(const QString &pName) const;
}; // VocabularyDatabase

Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::FieldAttributes)
#ifndef EDITION_FREE
Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::LanguageIds)
#endif

#endif // VOCABULARYDATABASE_H