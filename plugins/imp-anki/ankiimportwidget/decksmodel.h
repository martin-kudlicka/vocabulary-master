#ifndef DECKSMODEL_H
#define DECKSMODEL_H

#include <QtCore/QAbstractItemModel>

class QSqlDatabase;

class DecksModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
             DecksModel(const QSqlDatabase *database, QObject *parent = nullptr);
    virtual ~DecksModel() override;

    quintptr deckId(quintptr row) const;

  private:
    enum class Column
    {
      Id,
      Count
    };
    enum class ColumnPosition
    {
      N1
    };

    const QSqlDatabase *_database;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const override;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const override;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const override;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const override;
    virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const override;
};

#endif