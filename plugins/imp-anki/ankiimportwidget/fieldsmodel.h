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

             FieldsModel(const QSqlDatabase *database, QObject *parent = Q_NULLPTR);
    virtual ~FieldsModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    qlonglong fieldId   (quintptr row) const;
    void      setModelId(qlonglong pModelId);

    virtual QVariant    data    (const QModelIndex &index, int role = Qt::DisplayRole)           const Q_DECL_OVERRIDE;
    virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const Q_DECL_OVERRIDE;

  private:
    enum class ColumnPosition
    {
      N1
    };

          qlonglong     _modelId;
    const QSqlDatabase *_database;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
};

#endif