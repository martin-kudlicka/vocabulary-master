#ifndef VOCABULARYMODEL_H
#define VOCABULARYMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QSharedPointer>

class Vocabulary;

class VocabularyModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    VocabularyModel(const QSharedPointer<Vocabulary> &vocabulary, quintptr categoryId, QObject *parent = Q_NULLPTR);

            void addRow     ();
    virtual int  columnCount(const QModelIndex &parent = QModelIndex()) const                          Q_DECL_OVERRIDE;
            void removeRow  (quintptr row);
    virtual int  rowCount   (const QModelIndex &parent = QModelIndex()) const                          Q_DECL_OVERRIDE;
    virtual bool setData    (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

  private:
    const quintptr                   _categoryId;
    const QSharedPointer<Vocabulary> _vocabulary;

    virtual ~VocabularyModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual QVariant      data      (const QModelIndex &index, int role = Qt::DisplayRole)                 const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags     (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
    virtual QVariant      headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};

#endif