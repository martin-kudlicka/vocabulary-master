#ifndef MODELSMODEL_H
#define MODELSMODEL_H

#include <QtCore/QAbstractItemModel>

class QSqlDatabase;

class ModelsModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
             ModelsModel(const QSqlDatabase *database, QObject *parent = nullptr);
    virtual ~ModelsModel() override;

    void      deckId (quintptr deckId);
    qlonglong modelId(quintptr row) const;

  private:
    enum class Column
    {
      Name,
      Count
    };
    enum class ColumnPosition
    {
      N1
    };

          quintptr      _deckId;
    const QSqlDatabase *_database;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const override;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const override;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const override;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const override;
    virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const override;
};

#endif