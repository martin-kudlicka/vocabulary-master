#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QtCore/QAbstractItemModel>

class Vocabulary;

class CategoriesModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
             CategoriesModel(const Vocabulary *vocabulary, QObject *parent = Q_NULLPTR);
    virtual ~CategoriesModel() Q_DECL_OVERRIDE;

  private:
    enum class Column
    {
      Name,
      Count
    };

    const Vocabulary *_vocabulary;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const Q_DECL_OVERRIDE;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const Q_DECL_OVERRIDE;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
    virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
};

#endif