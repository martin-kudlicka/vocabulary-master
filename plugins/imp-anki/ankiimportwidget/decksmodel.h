#ifndef DECKSMODEL_H
#define DECKSMODEL_H

class QSqlDatabase;

class DecksModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
             DecksModel(const QSqlDatabase *database);
    virtual ~DecksModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

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

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const Q_DECL_OVERRIDE;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const Q_DECL_OVERRIDE;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
    virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
};

#endif