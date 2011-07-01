#include "vocabulary.h"

#include <QtCore/QStringList>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>
#include <QtSql/QSqlRecord>
#include <QtCore/QFile>

const QString COLUMN_CATEGORYID = "categoryid";
const QString COLUMN_ID = "id";
//const QString COLUMN_KEY = "key";
const QString COLUMN_NAME = "name";
const QString COLUMN_PRIORITY = "priority";
/*const QString COLUMN_VALUE = "value";
const QString TABLE_SETTINGS = "settings";*/
const QString TABLE_CATEGORIES = "categories";
const QString TABLE_WORDS = "words";

const int Vocabulary::AddCategory(const QString &pName) const
{
    QSqlQuery qsqQuery("INSERT INTO " + TABLE_CATEGORIES + " (" + COLUMN_NAME + ") VALUES ('" + pName + "')");
    return qsqQuery.lastInsertId().toInt();
} // AddCategory

const Vocabulary::tCategoryIdList Vocabulary::GetCategoryIds() const
{
    tCategoryIdList tcilCategories;

    QSqlQuery qsqQuery("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    while (qsqQuery.next()) {
        tcilCategories.append(qsqQuery.value(0).toInt());
    } // while

    return tcilCategories;
} // GetCategoryIds

const QString Vocabulary::GetCategoryName(const int &pCategoryId) const
{
    QSqlQuery qsqQuery("SELECT " + COLUMN_NAME + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
    if (qsqQuery.next()) {
        return qsqQuery.value(0).toString();
    } else {
        return QString();
    } // if else
} // GetCategoryName

const QString &Vocabulary::GetVocabularyFile() const
{
    return _qsVocabularyFile;
} // GetVocabularyFile

const QString Vocabulary::GetWord(const int &pCategoryId, const int &pRow, const QString &pLanguage) const
{
    QSqlQuery qsqQuery("SELECT " + COLUMN_ID + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
    qsqQuery.seek(pRow);
    return qsqQuery.value(qsqQuery.record().indexOf(pLanguage)).toString();
} // GetWord

const int Vocabulary::GetWordCount(const int &pCategoryId) const
{
    QSqlQuery qsqQuery("SELECT " + COLUMN_ID + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
    return qsqQuery.size();
} // GetWordCount

const void Vocabulary::Initialize() const
{
    QSqlQuery qsqQuery;

    /*qsqQuery.exec("CREATE TABLE " + TABLE_SETTINGS + " ("
                  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                  + COLUMN_KEY + " TEXT,"
                  + COLUMN_VALUE + " TEXT)");*/
    qsqQuery.exec("CREATE TABLE " + TABLE_CATEGORIES + " ("
                  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                  + COLUMN_NAME + " TEXT)");
    qsqQuery.exec("CREATE TABLE " + TABLE_WORDS + " ("
                  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                  + COLUMN_CATEGORYID + " INTEGER,"
                  + COLUMN_LANG1 + " TEXT,"
                  + COLUMN_LANG2 + " TEXT,"
                  + COLUMN_PRIORITY + " INTEGER)");
} // Initialize

const bool Vocabulary::IsOpen() const
{
    return _qsdDatabase.isOpen();
} // IsOpen

const void Vocabulary::Open(const QString &pFilePath)
{
    if (!QFile::exists(pFilePath)) {
        return;
    } // if

    _qsVocabularyFile = pFilePath;

    if (_qsdDatabase.isOpen()) {
        _qsdDatabase.close();
    } // if

    _qsdDatabase.setDatabaseName(_qsVocabularyFile);
    _qsdDatabase.open();

    if (_qsdDatabase.tables().isEmpty()) {
        Initialize();
    } // if
} // Open

Vocabulary::Vocabulary()
{
    // create empty database
    _qsdDatabase = QSqlDatabase::addDatabase("QSQLITE");
} // Vocabulary