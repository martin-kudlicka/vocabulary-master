#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <QtSql/QSqlDatabase>
#include <QtCore/QList>

const QString KEY_LANGUAGE1 = "language1";
const QString KEY_LANGUAGE2 = "language2";
const QString KEY_LEARNINGTEMPLATE1 = "learningtemplate1";
const QString KEY_LEARNINGTEMPLATE2 = "learningtemplate2";
#ifndef FREE
const QString KEY_SPEECH1 = "speech1";
const QString KEY_SPEECH2 = "speech2";
const QString KEY_TRAYTEMPLATE1 = "traytemplate1";
const QString KEY_TRAYTEMPLATE2 = "traytemplate2";
const QString KEY_VOICE1 = "voice1";
const QString KEY_VOICE2 = "voice2";
#endif
const QString VARIABLE_MARK = "$";

class Vocabulary
{
    public:
        typedef QList<int> tCategoryIdList;
		typedef QList<int> tFieldIdList;

        enum eFieldAttribute {
            FieldAttributeNone
#ifndef FREE
            , FieldAttributeSpeech
#endif
        }; // eFieldAttribute
        Q_DECLARE_FLAGS(FieldAttributes, eFieldAttribute)

		enum eFieldLanguage {
			FieldLanguageUnknown,
			FieldLanguageLeft,
			FieldLanguageRight
#ifndef FREE
            , FieldLanguageCount
#endif
		}; // eFieldLanguage

		/*enum eFieldType {
			FieldTypeUnknown,
			FieldTypeTextEdit
		}; // eFieldType*/

        static const int NOT_FOUND = -1;

		~Vocabulary();
        Vocabulary();

        const int AddCategory(const QString &pName) const;
#ifndef FREE
        const void AddField() const;
#endif
		const void AddRecord(const int &pCategoryId) const;
        const void BeginEdit();
        const void EndEdit(const bool &pSave = true);
#ifndef FREE
        const bool GetCategoryEnabled(const int &pCategoryId) const;
#endif
        const tCategoryIdList GetCategoryIds() const;
        const QString GetCategoryName(const int &pCategoryId) const;
		const QString GetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const;
		const QString GetDataText(const int &pRecordId, const int &pFieldId) const;
#ifndef FREE
        const FieldAttributes GetFieldAttributes(const int &pFieldId) const;
#endif
		const int GetFieldCount() const;
		const int GetFieldId(const int &pPosition) const;
		const tFieldIdList GetFieldIds() const;
		const eFieldLanguage GetFieldLanguage(const int &pFieldId) const;
		const QString GetFieldName(const int &pFieldId) const;
        const QString GetFieldTemplateName(const int &pFieldId) const;
		//const eFieldType GetFieldType(const int &pFieldId) const;
#ifndef FREE
        const QString GetLanguageName(const eFieldLanguage &pLanguage) const;
#endif
        const QString GetName() const;
		const int GetRecordCategory(const int &pRecordId) const;
		const int GetRecordCount() const;
		const int GetRecordCount(const int &pCategoryId) const;
        const int GetRecordCount(const bool &pEnabled) const;
		const int GetRecordId(const int &pRow) const;
		const int GetRecordId(const int &pCategoryId, const int &pRow) const;
		const int GetRow(const int &pRecordId, const int &pCategoryId) const;
		const QString GetSettings(const QString &pKey) const;
        const QString &GetVocabularyFile() const;
        const bool IsOpen() const;
		const void New(const QString &pFilePath);
        const void Open(const QString &pFilePath);
        const void RemoveCategory(const int &pCategoryId) const;
#ifndef FREE
		const void RemoveField(const int &pFieldId) const;
#endif
		const void RemoveRecord(const int &pCategoryId, const int &pRow) const;
		const int Search(const QString &pWord, const int &pStartId) const;
#ifndef FREE
        const void SetCategoryEnabled(const int &pCategoryId, const bool &pEnabled) const;
#endif
		const void SetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId, const QString &pData) const;
#ifndef FREE
        const void SetFieldAttributes(const int &pFieldId, const FieldAttributes &pAttributes) const;
        const void SetFieldLanguage(const int &pFieldId, const eFieldLanguage &pLanguage) const;
        const void SetFieldName(const int &pFieldId, const QString &pName) const;
        const void SetFieldTemplateName(const int &pFieldId, const QString &pTemplateName) const;
#endif
		const void SetSettings(const QString &pKey, const QString &pValue) const;
#ifndef FREE
		const void SwapFields(const int &pSourceId, const int &pDestinationId) const;
#endif

    private:
		enum eColumnPosition {
			ColumnPosition1,
			ColumnPosition2
		}; // eColumnPosition

        QSqlDatabase _qsdDatabase;
        QString _qsVocabularyFile;

		const void CloseDatabase();
		const void Initialize() const;
		const void OpenDatabase();
		const void RemoveRecord(const int &pRecordId) const;
#ifndef FREE
		const void Update(const QString &pTable, const int &pColumnId, const QSqlRecord &pRecord) const;
#endif
}; // Vocabulary

Q_DECLARE_OPERATORS_FOR_FLAGS(Vocabulary::FieldAttributes)

#endif // VOCABULARY_H