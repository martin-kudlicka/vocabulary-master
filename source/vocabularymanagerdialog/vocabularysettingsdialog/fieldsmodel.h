#ifndef FIELDSMODEL_H
#define FIELDSMODEL_H

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

             FieldsModel(const QSharedPointer<Vocabulary> &vocabulary);
    virtual ~FieldsModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    void addRow   ();
    void removeRow(quintptr row);
    void swap     (quintptr sourceRow, quintptr destinationRow);

    virtual QVariant    data    (const QModelIndex &index, int role = Qt::DisplayRole)           const Q_DECL_OVERRIDE;
    virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const Q_DECL_OVERRIDE;

  private:
    const QSharedPointer<Vocabulary> _vocabulary;

    virtual int           columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags      (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
    virtual QVariant      headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex   parent     (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
    virtual bool          setData    (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)   Q_DECL_OVERRIDE;
};

#endif