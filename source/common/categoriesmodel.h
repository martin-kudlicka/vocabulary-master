#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QtCore/QAbstractItemModel>

class Vocabulary;

class CategoriesModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
             CategoriesModel(const Vocabulary *vocabulary, QObject *parent = nullptr);
    virtual ~CategoriesModel() override;

  private:
    enum class Column
    {
      Name,
      Count
    };

    const Vocabulary *_vocabulary;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const override;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const override;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const override;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const override;
    virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const override;
};

#endif