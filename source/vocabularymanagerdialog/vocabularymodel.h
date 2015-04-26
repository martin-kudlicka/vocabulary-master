#ifndef VOCABULARYMODEL_H
#define VOCABULARYMODEL_H

#include <QtCore/QAbstractTableModel>
#include "vocabulary.h"

class VocabularyModel : public QAbstractTableModel
{
	Q_OBJECT

    public:
        VocabularyModel(Vocabulary *vocabulary, quint8 categoryId, QObject *parent = nullptr);

                void addRow     ();
		virtual int  columnCount(const QModelIndex &parent = QModelIndex()) const                      override;
                void removeRow  (quint32 row);
        virtual int  rowCount   (const QModelIndex &parent = QModelIndex()) const                      override;
		virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    private:
        const quint8      _categoryId;
              Vocabulary *_vocabulary;

		virtual ~VocabularyModel() override;

        virtual QVariant      data      (const QModelIndex &index, int role = Qt::DisplayRole)                 const override;
#ifndef EDITION_FREE
		virtual Qt::ItemFlags flags     (const QModelIndex &index)                                             const override;
#endif
        virtual QVariant      headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
}; // VocabularyModel

#endif // VOCABULARYMODEL_H