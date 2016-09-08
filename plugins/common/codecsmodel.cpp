#include "codecsmodel.h"

#include <QtCore/QTextCodec>

CodecsModel::CodecsModel(QObject *pParent /* nullptr */) : QAbstractItemModel(pParent), _codecs(QTextCodec::availableCodecs())
{
}

CodecsModel::~CodecsModel()
{
}

QVariant CodecsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            return _codecs.at(index.row());
        default:
            return QVariant();
    }
}

QModelIndex CodecsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
	return createIndex(row, column);
}

int CodecsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
	if (parent == QModelIndex())
	{
		return _codecs.size();
	}
	else
	{
		return 0;
	}
}

int CodecsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
	return ColumnCount;
}

QVariant CodecsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            return tr("Codec");
        default:
            return QVariant();
    }
}

QModelIndex CodecsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}