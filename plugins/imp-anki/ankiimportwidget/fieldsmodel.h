#ifndef FIELDSMODEL_H
#define FIELDSMODEL_H

#include <QtCore/QAbstractItemModel>

class QSqlDatabase;

class FieldsModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
    enum Column
    {
      Name,
      Mark,
      Count
    };

             FieldsModel(const QSqlDatabase *database, QObject *parent = nullptr);
    virtual ~FieldsModel() override;

    qlonglong fieldId   (quintptr row) const;
    void      setModelId(qlonglong pModelId);

    virtual QVariant    data    (const QModelIndex &index, int role = Qt::DisplayRole)           const override;
    virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const override;

  private:
    enum class ColumnPosition
    {
      N1
    };

          qlonglong     _modelId;
    const QSqlDatabase *_database;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const override;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const override;
};

#endif