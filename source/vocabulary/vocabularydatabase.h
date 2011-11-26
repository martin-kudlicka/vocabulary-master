#ifndef VOCABULARYDATABASE_H
#define VOCABULARYDATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtCore/QList>
#include <QtCore/QHash>
#ifndef FREE
# include "../plugins/common/tts-interface.h"
#endif

const QString VARIABLE_MARK = "$";

class VocabularyDatabase : public QObject
{
	Q_OBJECT
    Q_ENUMS(eFieldAttribute)
    Q_ENUMS(eFieldBuiltIn)
    Q_ENUMS(eFieldLanguage)
	Q_ENUMS(eFieldType)
	Q_ENUMS(eLanguageIds)

    public:
        typedef QHash<int, QString> tDataHash;
        typedef QHash<int, tDataHash> tRecordDataHash;
        typedef QList<int> tCategoryIdList;
		typedef QList<int> tFieldIdList;
        typedef QList<int> tLanguageIdList;

        enum eFieldAttribute {
            FieldAttributeNone,
#ifndef FREE
            FieldAttributeSpeech,
#endif
            FieldAttributeShow = 2,
            FieldAttributeBuiltIn = 4
        }; // eFieldAttribute
        Q_DECLARE_FLAGS(qfFieldAttributes, eFieldAttribute)
        enum eFieldBuiltIn {
            FieldBuiltInNone,
            FieldBuiltInEnabled,
			FieldBuiltInPriority
        }; // eFieldBuiltIn
		enum eFieldLanguage {
			FieldLanguageUnknown,
			FieldLanguageLeft,
			FieldLanguageRight,
            FieldLanguageAll = INT_MAX
		}; // eFieldLanguage
		enum eFieldType {
			FieldTypeUnknown,
			FieldTypeLineEdit,
			FieldTypeCheckBox,
			FieldTypeSpinBox
		}; // eFieldType
#ifndef FREE
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

#ifndef FREE
        const int GetCategoryCount() const;
        const bool GetCategoryEnabled(const int &pCategoryId) const;
        const int GetCategoryId(const int &pRow) const;
#endif
        const QString GetCategoryName(const int &pCategoryId) const;
#ifndef FREE
		const int GetCategoryPriority(const int &pCategoryId) const;
        const tLanguageIdList GetLanguageIds(const qfLanguageIds &pType) const;
#endif
        const QString GetLanguageLearningTemplate(const int &pLanguageId) const;
        const QString GetLanguageName(const int &pLanguageId) const;
#ifndef FREE
        const TTSInterface::eTTSPlugin GetLanguageSpeech(const int &pLanguageId) const;
		const QString GetLanguageTrayTemplate(const int &pLanguageId) const;
        const QString GetLanguageVoice(const int &pLanguageId) const;
#endif
        const QString GetName() const;
		const int GetRow(const int &pRecordId, const int &pCategoryId) const;
		const QString GetSettings(const QString &pKey) const;
#ifndef TRY
        const QString &GetVocabularyFile() const;
#endif
        const bool IsOpen() const;
		const int Search(const QString &pWord, const int &pStartId) const;
#ifndef FREE
        const void SetCategoryEnabled(const int &pCategoryId, const bool &pEnabled) const;
		const void SetCategoryPriority(const int &pCategoryId, const int &pPriority) const;
        const void SetLanguageLearningTemplate(const int &pLanguageId, const QString &pTemplate) const;
#endif
        const void SetLanguageName(const int &pLanguageId, const QString &pName) const;
#ifndef FREE
        const void SetLanguageSpeech(const int &pLanguageId, const TTSInterface::eTTSPlugin &pSpeech) const;
        const void SetLanguageTrayTemplate(const int &pLanguageId, const QString &pTemplate) const;
        const void SetLanguageVoice(const int &pLanguageId, const QString &pVoice) const;
#endif
		const void SetSettings(const QString &pKey, const QString &pValue) const;

    protected:
        typedef QList<int> tRecordIdList;

        const int AddCategory(const QString &pName) const;
#ifndef FREE
        const int AddField() const;
#endif
        const int AddRecord(const int &pCategoryId) const;
#ifndef FREE
        const int AddRecord(const int &pCategoryId, const QStringList &pData) const;
#endif
        const void BeginEdit();
		const void Close();
        const void EndEdit(const bool &pSave = true);
        const tCategoryIdList GetCategoryIds() const;
        /*const QString GetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const;
        const QString GetDataText(const int &pRecordId, const int &pFieldId) const;*/
        tRecordDataHash *GetDataText() const;
        const qfFieldAttributes GetFieldAttributes(const int &pFieldId) const;
#ifndef FREE
        const eFieldBuiltIn GetFieldBuiltIn(const int &pFieldId) const;
        const int GetFieldCount() const;
#endif
        //const int GetFieldId(const int &pPosition) const;
        const tFieldIdList GetFieldIds() const;
        const eFieldLanguage GetFieldLanguage(const int &pFieldId) const;
        const QString GetFieldName(const int &pFieldId) const;
        const QString GetFieldTemplateName(const int &pFieldId) const;
        const eFieldType GetFieldType(const int &pFieldId) const;
        /*const int GetRecordCategory(const int &pRecordId) const;
        const int GetRecordCount() const;
		const int GetRecordCount(const int &pCategoryId) const;
        const int GetRecordCount(const bool &pEnabled) const;
        const int GetRecordId(const int &pRow) const;
        const int GetRecordId(const int &pCategoryId, const int &pRow) const;*/
        const tRecordIdList GetRecordIds(const int &pCategoryId) const;
        const void New(
#ifndef TRY
			const QString &pFilePath
#endif
			);
#ifndef TRY
        const void Open(const QString &pFilePath);
#endif
        const void RemoveCategory(const int &pCategoryId) const;
#ifndef FREE
        const void RemoveField(const int &pFieldId) const;
#endif
        const void RemoveRecord(const int &pCategoryId, const int &pRow) const;
        //const void SetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId, const QString &pData) const;
        const void SetDataText(const int &pRecordId, const int &pFieldId, const QString &pData) const;
        const void SetFieldAttributes(const int &pFieldId, const qfFieldAttributes &pAttributes) const;
#ifndef FREE
        const void SetFieldLanguage(const int &pFieldId, const eFieldLanguage &pLanguage) const;
        const void SetFieldName(const int &pFieldId, const QString &pName) const;
        const void SetFieldTemplateName(const int &pFieldId, const QString &pTemplateName) const;
		const void SetRecordCategory(const int &pRecordId, const int &pCategoryId) const;
        const void SwapFields(const int &pSourceId, const int &pDestinationId) const;
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

#ifdef TRY
		static int _iMemoryVocabularies;
		int _iVocabularyNumber;
#endif
        QSqlDatabase _qsdDatabase;
#ifndef TRY
        QString _qsVocabularyFile;
#endif

        const int AddField(const QString &pTemplate, const QString &pName, const eFieldType &pType, const qfFieldAttributes &pAttributes, const eFieldBuiltIn &pBuiltIn, const eFieldLanguage &pLanguage) const;
        const void AddLanguage(const QString &pName, const QString &pLearningTemplate
#ifndef FREE
            , const QString &pTrayTemplate, const TTSInterface::eTTSPlugin &pTTSPlugin, const QString &pVoice
#endif
            ) const;
		const void CloseDatabase();
#ifndef TRY
        const tRecordIdList GetRecordIds() const;
#endif
		const void Initialize() const;
		const void OpenDatabase();
		const void RemoveRecord(const int &pRecordId) const;
#ifndef FREE
		const void Update(const QString &pTable, const int &pColumnId, const QSqlRecord &pRecord) const;
#endif
#ifndef TRY
        const void UpdateDatabase();
#endif

	signals:
		void SetOpenProgressValue(const int &pValue) const;
		void SetOpenProgressMax(const int &pMax) const;
		void SetVocabularyName(const QString &pName) const;
}; // VocabularyDatabase

Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::qfFieldAttributes)
#ifndef FREE
Q_DECLARE_OPERATORS_FOR_FLAGS(VocabularyDatabase::qfLanguageIds)
#endif

#endif // VOCABULARYDATABASE_H