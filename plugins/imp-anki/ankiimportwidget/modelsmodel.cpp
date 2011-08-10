#include "ankiimportwidget/modelsmodel.h"

#include <QtSql/QSqlQuery>

const QString COLUMN_DECKID = "deckId";
const QString COLUMN_DESCRIPTION = "description";
const QString COLUMN_ID = "id";
const QString COLUMN_NAME = "name";
const QString COLUMN_TAGS = "tags";
const QString TABLE_MODELS = "models";

int ModelsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant ModelsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            {
                QSqlQuery qsqQuery = _qsdAnki->exec("SELECT " + COLUMN_TAGS + ", " + COLUMN_NAME + ", " + COLUMN_DESCRIPTION + " FROM " + TABLE_MODELS + " WHERE " + COLUMN_DECKID + " = " + QString::number(_iDeckId));
                qsqQuery.seek(index.row());

                switch (index.column()) {
                    case ColumnTags:
                        return qsqQuery.value(ColumnPosition1);
                    case ColumnName:
                        return qsqQuery.value(ColumnPosition2);
                    case ColumnDescription:
                        return qsqQuery.value(ColumnPosition3);
                } // switch
            }
        default:
            return QVariant();
    } // switch
} // data

ModelsModel::ModelsModel(const QSqlDatabase *pAnki, QObject *pParent /* NULL */) : QAbstractItemModel(pParent)
{
    _qsdAnki = pAnki;

    _iDeckId = 0;
} // DecksModel

const qlonglong ModelsModel::GetModelId(const int &pRow) const
{
    QSqlQuery qsqQuery = _qsdAnki->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_MODELS + " WHERE " + COLUMN_DECKID + " = " + QString::number(_iDeckId));
    qsqQuery.seek(pRow);
    return qsqQuery.value(ColumnPosition1).toLongLong();
} // GetModelId

QVariant ModelsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (section) {
                case ColumnTags:
                    return tr("Tags");
                case ColumnName:
                    return tr("Name");
                case ColumnDescription:
                    return tr("Description");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex ModelsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
} // index

QModelIndex ModelsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent

int ModelsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex()) {
        QSqlQuery qsqQuery = _qsdAnki->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_MODELS + " WHERE " + COLUMN_DECKID + " = " + QString::number(_iDeckId));
        if (qsqQuery.last()) {
            return qsqQuery.at() + 1;
        } else {
            return 0;
        } // if else
    } else {
        return 0;
    } // if else
} // rowCount

const void ModelsModel::SetDeckId(const int &pDeckId)
{
    _iDeckId = pDeckId;
} // SetDeckId