#ifndef VOCABULARYMODEL_H
#define VOCABULARYMODEL_H

#include <QtCore/QAbstractTableModel>
#include "vocabulary.h"

class VocabularyModel : public QAbstractTableModel
{
	Q_OBJECT

    public:
        VocabularyModel(Vocabulary *vocabulary, quint8 categoryId, QObject *parent = NULL);

                void addRow     ();
		virtual int  columnCount(const QModelIndex &parent = QModelIndex()) const;
                void removeRow  (quint32 row);
        virtual int  rowCount   (const QModelIndex &parent = QModelIndex()) const;
		virtual bool setData    (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    private:
        const quint8      _categoryId;
              Vocabulary *_vocabulary;

		virtual ~VocabularyModel();

        virtual QVariant      data      (const QModelIndex &index, int role = Qt::DisplayRole)                 const;
#ifndef EDITION_FREE
		virtual Qt::ItemFlags flags     (const QModelIndex &index)                                             const;
#endif
        virtual QVariant      headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
}; // VocabularyModel

#endif // VOCABULARYMODEL_H