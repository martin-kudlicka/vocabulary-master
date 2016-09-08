#ifndef FIELDSMODEL_H
#define FIELDSMODEL_H

#include <QtCore/QAbstractItemModel>

class Vocabulary;

class FieldsModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
    enum class Column
    {
      TemplateName,
      Name,
      Speech,
      Show,
      Language,
      Count
    };

             FieldsModel(Vocabulary *vocabulary, QObject *parent = nullptr);
    virtual ~FieldsModel() override;

    void addRow   ();
    void removeRow(quintptr row);
    void swap     (quintptr sourceRow, quintptr destinationRow);

    virtual QVariant    data    (const QModelIndex &index, int role = Qt::DisplayRole)           const override;
    virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const override;

  private:
    Vocabulary *_vocabulary;

    virtual int           columnCount(const QModelIndex &parent = QModelIndex())                            const override;
    virtual Qt::ItemFlags flags      (const QModelIndex &index)                                             const override;
    virtual QVariant      headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QModelIndex   parent     (const QModelIndex &index)                                             const override;
    virtual bool          setData    (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)   override;
};

#endif