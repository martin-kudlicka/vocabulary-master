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

                 CodecsModel(QObject *parent = nullptr);
		virtual ~CodecsModel() override;

		virtual QVariant    data    (const QModelIndex &index, int role = Qt::DisplayRole)           const override;
		virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const override;
		virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const override;

    private:
        QList<QByteArray> _codecs;

        virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const override;
        virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        virtual QModelIndex parent     (const QModelIndex &index)                                             const override;
}; // CodecsModel

#endif // CODECSMODEL_H