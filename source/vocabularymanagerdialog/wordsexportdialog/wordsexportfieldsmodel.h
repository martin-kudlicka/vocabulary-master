#ifndef WORDSEXPORTFIELDSMODEL_H
#define WORDSEXPORTFIELDSMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QSharedPointer>

class Vocabulary;

class WordsExportFieldsModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
    enum class Column
    {
      Language,
      Name,
      Mark,
      Count
    };

             WordsExportFieldsModel(const QSharedPointer<Vocabulary> &vocabulary);
    virtual ~WordsExportFieldsModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const Q_DECL_OVERRIDE;

  private:
    const QSharedPointer<Vocabulary> _vocabulary;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const Q_DECL_OVERRIDE;
            quintptr    fieldId    (quintptr row)                                                         const;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
};

#endif