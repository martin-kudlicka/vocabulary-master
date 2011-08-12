#ifndef CODECSMODEL_H
#define CODECSMODEL_H

#include <QtCore/QAbstractItemModel>

class CodecsModel : public QAbstractItemModel
{
    public:
        CodecsModel(QObject *pParent = NULL);

    private:
        enum eColumn {
            ColumnCodec,
            ColumnCount
        }; // eColumn

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &index) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
}; // CodecsModel

#endif // CODECSMODEL_H