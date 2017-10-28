#ifndef CODECSMODEL_H
#define CODECSMODEL_H

#include <QtCore/QAbstractItemModel>

class CodecsModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
    enum class Column
    {
      Codec,
      Count
    };

             CodecsModel(QObject *parent = Q_NULLPTR);
    virtual ~CodecsModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual QVariant    data    (const QModelIndex &index, int role = Qt::DisplayRole)           const Q_DECL_OVERRIDE;
    virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const Q_DECL_OVERRIDE;

  private:
    QList<QByteArray> _codecs;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
};

#endif