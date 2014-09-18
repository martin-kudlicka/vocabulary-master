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
    Q_ENUMS(FieldAttribute)
    Q_ENUMS(FieldBuiltIn)
    Q_ENUMS(FieldLanguage)
	Q_ENUMS(FieldType)
	Q_ENUMS(eLanguageIds)

    public:
        typedef QHash<int, QString> tDataHash;
        typedef QHash<int, tDataHash> RecordDataHash;
        typedef QList<int> CategoryIdList;
		typedef QList<int> FieldIdList;
        typedef QList<int> tLanguageIdList;

        enum FieldAttribute {
            FieldAttributeNone,
#ifndef EDITION_FREE
            FieldAttributeSpeech,
#endif
            FieldAttributeShow = 2,
            FieldAttributeBuiltIn = 4
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
            FieldLanguageAll = INT_MAX
		}; // FieldLanguage
		enum FieldType {
			FieldTypeUnknown,
			FieldTypeLineEdit,
			FieldTypeCheckBox,
			FieldTypeSpinBox
		}; // FieldType
#ifndef EDITION_FREE
		enum eLanguageIds {
			LanguageIdsNone,
			LanguageIdsUserDefined,
			LanguageIdsAllOnly,
			LanguageIdsAll = 4
		}; // eLanguageIds
		Q_DECLARE_FLAGS(qfLanguageIds, eLanguageIds)
#endif

        static const int NOT_FOUND = -1;

		~VocabularyDatabase();
        VocabularyDatabase(QObject *pParent = NULL);

#ifndef EDITION_FREE
        const int GetCategoryCount() const;
        const bool GetCategoryEnabled(const int &pCategoryId) const;
        const int GetCategoryId(const int &pRow) const;
#endif
        const QString GetCategoryName(const int &pCategoryId) const;
#ifndef EDITION_FREE
		const int GetCategoryPriority(const int &pCategoryId) const;
        const tLanguageIdList GetLanguageIds(const qfLanguageIds &pType) const;
#endif
        const QString GetLanguageLearningTemplate(const int &pLanguageId) const;
        const QString GetLanguageName(const int &pLanguageId) const;
#ifndef EDITION_FREE
        const TTSInterface::TTSPlugin GetLanguageSpeech(const int &pLanguageId) const;
		const QString GetLanguageTrayTemplate(const int &pLanguageId) const;
        const QString GetLanguageVoice(const int &pLanguageId) const;
#endif
        const QString GetName() const;
		const int GetRow(const int &pRecordId, const int &pCategoryId) const;
		const QString GetSettings(const QString &pKey) const;
#ifndef EDITION_TRY
        const QString &GetVocabularyFile() const;
#endif
        const bool IsOpen() const;
		const int Search(const QString &pWord, const int &pStartId) const;
#ifndef EDITION_FREE
        const void SetCategoryEnabled(const int &pCategoryId, const bool &pEnabled) const;
		const void SetCategoryPriority(const int &pCategoryId, const int &pPriority) const;
        const void SetLanguageLearningTemplate(const int &pLanguageId, const QString &pTemplate) const;
#endif
        const void SetLanguageName(const int &pLanguageId, const QString &pName) const;
#ifndef EDITION_FREE
        const void SetLanguageSpeech(const int &pLanguageId, const TTSInterface::TTSPlugin &pSpeech) const;
        const void SetLanguageTrayTemplate(const int &pLanguageId, const QString &pTemplate) const;
        const void SetLanguageVoice(const int &pLanguageId, const QString &pVoice) const;
#endif
		const void SetSettings(const QString &pKey, const QString &pValue) const;

    protected:
        typedef QList<quint32> RecordIdList;

        const int addCategory(const QString &pName) const;
#ifndef EDITION_FREE
        const int addField() const;
#endif
        const int addRecord(const int &pCategoryId) const;
#ifndef EDITION_FREE
        const int addRecord(const int &pCategoryId, const QStringList &pData) const;
#endif
        const void beginEdit();
		const void close();
        const void endEdit(const bool &pSave = true);
        const CategoryIdList categoryIds() const;
        /*const QString dataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const;
        const QString dataText(const int &pRecordId, const int &pFieldId) const;*/
        RecordDataHash *dataText() const;
        const FieldAttributes fieldAttributes(const int &pFieldId) const;
#ifndef EDITION_FREE
        const FieldBuiltIn fieldBuiltIn(const int &pFieldId) const;
        const int fieldCount() const;
#endif
        //const int GetFieldId(const int &pPosition) const;
        const FieldIdList fieldIds() const;
        const FieldLanguage fieldLanguage(const int &pFieldId) const;
        const QString fieldName(const int &pFieldId) const;
        const QString fieldTemplateName(const int &pFieldId) const;
        const FieldType fieldType(const int &pFieldId) const;
        /*const int GetRecordCategory(const int &pRecordId) const;
        const int GetRecordCount() const;
		const int GetRecordCount(const int &pCategoryId) const;
        const int GetRecordCount(const bool &pEnabled) const;
        const int GetRecordId(const int &pRow) const;
        const int GetRecordId(const int &pCategoryId, const int &pRow) const;*/
        const RecordIdList recordIds(const int &pCategoryId) const;
        const void new2(
#ifndef EDITION_TRY
			const QString &pFilePath
#endif
			);
#ifndef EDITION_TRY
        const void open(const QString &pFilePath);
#endif
        const void removeCategory(const int &pCategoryId) const;
#ifndef EDITION_FREE
        const void removeField(const int &pFieldId) const;
#endif
        const void removeRecord(const int &pCategoryId, const int &pRow) const;
        //const void setDataText(const int &pCategoryId, const int &pRow, const int &pFieldId, const QString &pData) const;
        const void setDataText(const int &pRecordId, const int &pFieldId, const QString &pData) const;
        const void setFieldAttributes(const int &pFieldId, const FieldAttributes &pAttributes) const;
#ifndef EDITION_FREE
        const void setFieldLanguage(const int &pFieldId, const FieldLanguage &pLanguage) const;
        const void setFieldName(const int &pFieldId, const QString &pName) const;
        const void setFieldTemplateName(const int &pFieldId, const QString &pTemplateName) const;
		const void SetRecordCategory(const int &pRecordId, const int &pCategoryId) const;
        const void swapFields(const int &pSourceId, const int &pDestinationId) const;
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

        const int addField(const QString &pTemplate, const QString &pName, const FieldType &pType, const FieldAttributes &pAttributes, const FieldBuiltIn &pBuiltIn, const FieldLanguage &pLanguage) const;
        const void AddLanguage(const QString &pName, const QString &pLearningTemplate
#ifndef EDITION_FREE
            , const QString &pTrayTemplate, const TTSInterface::TTSPlugin &pTTSPlugin, const QString &pVoice
#endif
            ) const;
		const void CloseDatabase();
#ifndef EDITION_TRY
        const RecordIdList recordIds() const;
#endif
		const void Initialize() const;
		const void OpenDatabase();
		const void removeRecord(const int &pRecordId) const;
#ifndef EDITION_FREE
		const void Update(const QString &pTable, const int &pColumnId, const QSqlRecord &pRecord) const;
#endif
#ifndef EDITION_TRY
        const void UpdateDatabase();
#endif

	signals:
		void SetOpenProgressValue(const int &pValue) const;
		void SetOpenProgressMax(const int &pMax) const;
		void SetVocabularyName(const QString &pName) const;
}; // VocabularyDatabase

Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::FieldAttributes)
#ifndef EDITION_FREE
Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::qfLanguageIds)
#endif

#endif // VOCABULARYDATABASE_H