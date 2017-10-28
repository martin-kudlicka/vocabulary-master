#ifndef VOCABULARYORGANIZERMODEL_H
#define VOCABULARYORGANIZERMODEL_H

#include <QtCore/QAbstractTableModel>

class VocabularyOrganizer;

class VocabularyOrganizerModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    enum class Column
    {
      VocabularyFile,
      Enabled,
      Count
    };

             VocabularyOrganizerModel(VocabularyOrganizer *organizer, QWidget *parent);
    virtual ~VocabularyOrganizerModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    void addRow   ();
    void removeRow(quintptr row);

  private:
    QWidget             *_parent;
    VocabularyOrganizer *_organizer;

    virtual int           columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual QVariant      data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags      (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
    virtual QVariant      headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual int           rowCount   (const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual bool          setData    (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)   Q_DECL_OVERRIDE;
};

#endif