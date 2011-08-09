#include "ankiimportwidget/decksmodel.h"

#include <QtSql/QSqlQuery>

const QString COLUMN_DESCRIPTION = "description";
const QString COLUMN_ID = "id";
const QString COLUMN_FACTCOUNT = "factCount";
const QString TABLE_DECKS = "decks";

int DecksModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant DecksModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            {
                QSqlQuery qsqQuery = _qsdAnki->exec("SELECT " + COLUMN_ID + ", " + COLUMN_DESCRIPTION + ", " + COLUMN_FACTCOUNT + " FROM " + TABLE_DECKS);
                qsqQuery.seek(index.row());

                switch (index.column()) {
                    case ColumnId:
                        return qsqQuery.value(ColumnPosition1);
                    case ColumnDescription:
                        return qsqQuery.value(ColumnPosition2);
                    case ColumnFacts:
                        return qsqQuery.value(ColumnPosition3);
                } // switch
            }
        default:
            return QVariant();
    } // switch
} // data

DecksModel::DecksModel(const QSqlDatabase *pAnki, QObject *pParent /* NULL */) : QAbstractItemModel(pParent)
{
    _qsdAnki = pAnki;
} // DecksModel

QVariant DecksModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (section) {
                case ColumnId:
                    return "#";
                case ColumnDescription:
                    return tr("Description");
                case ColumnFacts:
                    return tr("Facts");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex DecksModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
} // index

QModelIndex DecksModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent

int DecksModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex()) {
        QSqlQuery qsqQuery = _qsdAnki->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DECKS);
        if (qsqQuery.last()) {
            return qsqQuery.at() + 1;
        } else {
            return 0;
        } // if else
    } else {
        return 0;
    } // if else
} // rowCount