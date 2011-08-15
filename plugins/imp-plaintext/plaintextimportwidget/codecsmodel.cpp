#include "plaintextimportwidget/codecsmodel.h"

#include <QtCore/QTextCodec>

CodecsModel::CodecsModel(QObject *pParent /* NULL */) : QAbstractItemModel(pParent)
{
    _qlCodecs = QTextCodec::availableCodecs();
} // CodecsModel

int CodecsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant CodecsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            return _qlCodecs.at(index.row());
        default:
            return QVariant();
    } // switch
} // data

QVariant CodecsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            return tr("Codec");
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex CodecsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
} // index

QModelIndex CodecsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent

int CodecsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex()) {
        return _qlCodecs.size();
    } else {
        return 0;
    } // if else
} // rowCount