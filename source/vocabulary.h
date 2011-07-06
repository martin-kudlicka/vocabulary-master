#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <QtSql/QSqlDatabase>
#include <QtCore/QList>

const QString COLUMN_LANG1 = "lang1";
const QString COLUMN_LANG2 = "lang2";
const QString COLUMN_NOTE1 = "note1";
const QString COLUMN_NOTE2 = "note2";
const QString COLUMN_SPEECH1 = "speech1";
const QString COLUMN_SPEECH2 = "speech2";

class Vocabulary
{
    public:
        typedef QList<int> tCategoryIdList;

        Vocabulary();

        const int AddCategory(const QString &pName) const;
		const void AddWord(const int &pCategoryId) const;
        const tCategoryIdList GetCategoryIds() const;
        const QString GetCategoryName(const int &pCategoryId) const;
        const QString GetNote(const int &pRow, const QString &pNote) const;
        const QString GetNote(const int &pCategoryId, const int &pRow, const QString &pNote) const;
		const QString GetSettings(const QString &pKey) const;
        const QString &GetVocabularyFile() const;
		const QString GetWord(const int &pRow, const QString &pLanguage) const;
        const QString GetWord(const int &pCategoryId, const int &pRow, const QString &pLanguage) const;
		const int GetWordCount() const;
        const int GetWordCount(const int &pCategoryId) const;
        const bool IsOpen() const;
		const void New(const QString &pFilePath);
        const void Open(const QString &pFilePath);
		const void SetSettings(const QString &pKey, const QString &pValue) const;
		const void SetWord(const QString &pWord, const int &pCategoryId, const int &pRow, const QString &pLanguage) const;

    private:
        QSqlDatabase _qsdDatabase;
        QString _qsVocabularyFile;

        const void Initialize() const;
}; // Vocabulary

#endif // VOCABULARY_H