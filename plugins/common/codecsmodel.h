#ifndef CODECSMODEL_H
#define CODECSMODEL_H

#include <QtCore/QAbstractItemModel>

class CodecsModel : public QAbstractItemModel
{
	Q_OBJECT

    public:
		enum Column
		{
			ColumnCodec,
			ColumnCount
		}; // Column

        CodecsModel(QObject *parent = NULL);

		virtual QVariant    data(const QModelIndex &index, int role = Qt::DisplayRole)            const;
		virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		virtual int         rowCount(const QModelIndex &parent = QModelIndex())                   const;

    private:
        QList<QByteArray> _codecs;

        virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const;
        virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex parent     (const QModelIndex &index)                                             const;
}; // CodecsModel

#endif // CODECSMODEL_H