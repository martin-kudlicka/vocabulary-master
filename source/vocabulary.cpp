#include "vocabulary.h"

#include <QtCore/QStringList>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>
#include <QtSql/QSqlRecord>
#include <QtCore/QFile>

const QString COLUMN_CATEGORYID = "categoryid";
const QString COLUMN_ENABLED = "enabled";
const QString COLUMN_ID = "id";
const QString COLUMN_KEY = "key";
const QString COLUMN_NAME = "name";
const QString COLUMN_PRIORITY = "priority";
const QString COLUMN_VALUE = "value";
const QString TABLE_SETTINGS = "settings";
const QString TABLE_CATEGORIES = "categories";
const QString TABLE_WORDS = "words";

const int Vocabulary::AddCategory(const QString &pName) const
{
    QSqlQuery qsqQuery("INSERT INTO " + TABLE_CATEGORIES + " (" + COLUMN_NAME + ") VALUES ('" + pName + "')");
    return qsqQuery.lastInsertId().toInt();
} // AddCategory

const void Vocabulary::AddWord(const int &pCategoryId) const
{
	QSqlQuery qsqQuery("INSERT INTO " + TABLE_WORDS + " (" + COLUMN_CATEGORYID + ") VALUES (" + QString::number(pCategoryId) + ")");
} // AddWord

#ifndef FREE
const bool Vocabulary::GetCategoryEnabled(const int &pCategoryId) const
{
    QSqlQuery qsqQuery("SELECT " + COLUMN_ENABLED + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
    if (qsqQuery.next()) {
        return qsqQuery.value(0).toBool();
    } else {
        return true;
    } // if else
} // GetCategoryEnabled
#endif

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

#ifndef FREE
const QString Vocabulary::GetNote(const int &pRow, const QString &pNote) const
{
    QSqlQuery qsqQuery("SELECT " + pNote + " FROM " + TABLE_WORDS);
    qsqQuery.seek(pRow);
    return qsqQuery.value(0).toString();
} // GetNote

const QString Vocabulary::GetNote(const int &pCategoryId, const int &pRow, const QString &pNote) const
{
    QSqlQuery qsqQuery("SELECT " + pNote + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
    qsqQuery.seek(pRow);
    return qsqQuery.value(0).toString();
} // GetNote
#endif

const int Vocabulary::GetRow(const int &pWordId, const int &pCategoryId) const
{
    int iRow = 0;

    QSqlQuery qsqQuery("SELECT " + COLUMN_ID + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
    while (qsqQuery.next()) {
        if (qsqQuery.value(0).toInt() == pWordId) {
            return iRow;
        } else {
            iRow++;
        } // if else
    } // while

    return -1;
} // GetRow

const QString Vocabulary::GetSettings(const QString &pKey) const
{
	QSqlQuery qsqQuery("SELECT " + COLUMN_VALUE + " FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + pKey + "'");
	if (qsqQuery.next()) {
		return qsqQuery.value(0).toString();
	} else {
		return QString();
	} // if else
} // GetSettings

const QString &Vocabulary::GetVocabularyFile() const
{
    return _qsVocabularyFile;
} // GetVocabularyFile

const QString Vocabulary::GetWord(const int &pRow, const QString &pLanguage) const
{
    QSqlQuery qsqQuery("SELECT " + pLanguage + " FROM " + TABLE_WORDS);
    qsqQuery.seek(pRow);
    return qsqQuery.value(0).toString();
} // GetWord

const QString Vocabulary::GetWord(const int &pCategoryId, const int &pRow, const QString &pLanguage) const
{
    QSqlQuery qsqQuery("SELECT " + pLanguage + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
    qsqQuery.seek(pRow);
    return qsqQuery.value(0).toString();
} // GetWord

const int Vocabulary::GetWordCategory(const int &pWordId) const
{
    QSqlQuery qsqQuery("SELECT " + COLUMN_CATEGORYID + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_ID + " = " + QString::number(pWordId));
    if (qsqQuery.next()) {
        return qsqQuery.value(0).toInt();
    } else {
        return -1;
    } // if else
} // GetWordCategory

const int Vocabulary::GetWordCount() const
{
	QSqlQuery qsqQuery("SELECT " + COLUMN_ID + " FROM " + TABLE_WORDS);
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetWordCount

const int Vocabulary::GetWordCount(const int &pCategoryId) const
{
    QSqlQuery qsqQuery("SELECT " + COLUMN_ID + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetWordCount

const int Vocabulary::GetWordId(const int &pCategoryId, const int &pRow) const
{
    QSqlQuery qsqQuery("SELECT " + COLUMN_ID + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
    qsqQuery.seek(pRow);
    return qsqQuery.value(0).toInt();
} // GetWordId

const void Vocabulary::Initialize() const
{
    QSqlQuery qsqQuery;

    qsqQuery.exec("CREATE TABLE " + TABLE_SETTINGS + " ("
                  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                  + COLUMN_KEY + " TEXT,"
                  + COLUMN_VALUE + " TEXT)");
    qsqQuery.exec("CREATE TABLE " + TABLE_CATEGORIES + " ("
                  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                  + COLUMN_NAME + " TEXT,"
				  + COLUMN_PRIORITY + " INTEGER,"
				  + COLUMN_ENABLED + " INTEGER)");
    qsqQuery.exec("CREATE TABLE " + TABLE_WORDS + " ("
                  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                  + COLUMN_CATEGORYID + " INTEGER,"
                  + COLUMN_LANG1 + " TEXT,"
                  + COLUMN_NOTE1 + " TEXT,"
                  + COLUMN_LANG2 + " TEXT,"
                  + COLUMN_NOTE2 + " TEXT,"
                  + COLUMN_PRIORITY + " INTEGER,"
				  + COLUMN_ENABLED + " INTEGER)");
} // Initialize

const bool Vocabulary::IsOpen() const
{
    return _qsdDatabase.isOpen();
} // IsOpen

const void Vocabulary::New(const QString &pFilePath)
{
	_qsVocabularyFile = pFilePath;

	if (_qsdDatabase.isOpen()) {
		_qsdDatabase.close();
	} // if

	if (QFile::exists(pFilePath)) {
		QFile::remove(pFilePath);
	} // if

	_qsdDatabase.setDatabaseName(_qsVocabularyFile);
	_qsdDatabase.open();

	Initialize();
} // New

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
} // Open

const void Vocabulary::RemoveCategory(const int &pCategoryId) const
{
    QSqlQuery qsqQuery;

    qsqQuery.exec("DELETE FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
    qsqQuery.exec("DELETE FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
} // RemoveCategory

const void Vocabulary::RemoveWord(const int &pCategoryId, const int &pRow) const
{
    QSqlQuery qsqQuery;

    qsqQuery.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
    qsqQuery.seek(pRow);
    int iColumnId = qsqQuery.value(0).toInt();

    qsqQuery.exec("DELETE FROM " + TABLE_WORDS + " WHERE " + COLUMN_ID + " = " + QString::number(iColumnId));
} // RemoveWord

const int Vocabulary::Search(const QString &pWord, const int &pStartId) const
{
    QString qsWordLike = '%' + pWord + '%';
    QSqlQuery qsqQuery("SELECT " + COLUMN_ID + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_LANG1 + " LIKE '" + qsWordLike + "' OR " + COLUMN_LANG2 + " LIKE '" + qsWordLike + "' ORDER BY " + COLUMN_ID);
    if (!qsqQuery.next()) {
        return SEARCH_NOT_FOUND;
    } // if

    do {
        if (qsqQuery.value(0).toInt() >= pStartId) {
            return qsqQuery.value(0).toInt();
        } // if
    } while (qsqQuery.next());

    qsqQuery.seek(0);
    return qsqQuery.value(0).toInt();
} // Search

#ifndef FREE
const void Vocabulary::SetCategoryEnabled(const int &pCategoryId, const bool &pEnabled) const
{
    QSqlQuery qsqQuery("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_ENABLED + " = " + QString::number(pEnabled) + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
} // SetCategoryEnabled

const void Vocabulary::SetNote(const QString &pNote, const int &pCategoryId, const int &pRow, const QString &pLanguage) const
{
	QSqlQuery qsqQuery;

	qsqQuery.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	int iColumnId = qsqQuery.value(0).toInt();

	qsqQuery.exec("UPDATE " + TABLE_WORDS + " SET " + pLanguage + " = '" + pNote + "' WHERE " + COLUMN_ID + " = " + QString::number(iColumnId));
} // SetNote
#endif

const void Vocabulary::SetSettings(const QString &pKey, const QString &pValue) const
{
	if (GetSettings(pKey).isEmpty()) {
		QSqlQuery qsqQuery("INSERT INTO " + TABLE_SETTINGS + " (" + COLUMN_KEY + ", " + COLUMN_VALUE + ") VALUES ('" + pKey + "', '" + pValue + "')");
	} else {
		QSqlQuery qsqQuery("UPDATE " + TABLE_SETTINGS + " SET " + COLUMN_VALUE + " = '" + pValue + "' WHERE " + COLUMN_KEY + " = '" + pKey + "'");
	} // if else
} // SetSettings

const void Vocabulary::SetWord(const QString &pWord, const int &pCategoryId, const int &pRow, const QString &pLanguage) const
{
	QSqlQuery qsqQuery;

	qsqQuery.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_WORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	int iColumnId = qsqQuery.value(0).toInt();

	qsqQuery.exec("UPDATE " + TABLE_WORDS + " SET " + pLanguage + " = '" + pWord + "' WHERE " + COLUMN_ID + " = " + QString::number(iColumnId));
} // SetWord

Vocabulary::Vocabulary()
{
    // create empty database
    _qsdDatabase = QSqlDatabase::addDatabase("QSQLITE");
} // Vocabulary