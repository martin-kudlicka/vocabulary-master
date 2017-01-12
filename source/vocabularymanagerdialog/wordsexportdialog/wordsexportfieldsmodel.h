#ifndef WORDSEXPORTFIELDSMODEL_H
#define WORDSEXPORTFIELDSMODEL_H

#include <QtCore/QAbstractItemModel>

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

             WordsExportFieldsModel(const Vocabulary *vocabulary, QObject *parent = Q_NULLPTR);
    virtual ~WordsExportFieldsModel() Q_DECL_OVERRIDE;

    virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const Q_DECL_OVERRIDE;

  private:
    const Vocabulary *_vocabulary;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const Q_DECL_OVERRIDE;
            quintptr    fieldId    (quintptr row)                                                         const;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
};

#endif