#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <QtSql/QSqlDatabase>

class Vocabulary
{
    public:
        typedef QList<int> tCategoryIdList;

        Vocabulary();

        const int AddCategory(const QString &pName) const;
        const tCategoryIdList GetCategoryIds() const;
        const QString GetCategoryName(const int &pCategoryId) const;
        const bool IsOpen() const;
        const void Open(const QString &pFilePath);

    private:
        QSqlDatabase _qsdDatabase;

        const void Initialize() const;
}; // Vocabulary

#endif // VOCABULARY_H