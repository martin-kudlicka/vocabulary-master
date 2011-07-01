#include "vocabulary.h"

#include <QtCore/QStringList>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>

const QString COLUMN_ID = "id";
//const QString COLUMN_KEY = "key";
const QString COLUMN_NAME = "name";
/*const QString COLUMN_VALUE = "value";
const QString TABLE_SETTINGS = "settings";*/
const QString TABLE_CATEGORIES = "categories";

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
} // Initialize

const bool Vocabulary::IsOpen() const
{
    return _qsdDatabase.isOpen();
} // IsOpen

const void Vocabulary::Open(const QString &pFilePath)
{
    if (_qsdDatabase.isOpen()) {
        _qsdDatabase.close();
    } // if

    _qsdDatabase.setDatabaseName(pFilePath);
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