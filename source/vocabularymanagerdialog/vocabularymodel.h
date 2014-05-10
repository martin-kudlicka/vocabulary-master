#ifndef VOCABULARYMODEL_H
#define VOCABULARYMODEL_H

#include <QtCore/QAbstractTableModel>
#include "vocabulary.h"

class VocabularyModel : public QAbstractTableModel
{
	Q_OBJECT

    public:
        VocabularyModel(Vocabulary *pVocabulary, const int &pCategoryId, QObject *pParent = NULL);

        const void AddRow();
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        const void RemoveRow(const int &pRow);
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
		virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    private:
        int _iCategoryId;
        Vocabulary *_vVocabulary;

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
#ifndef EDITION_FREE
		virtual Qt::ItemFlags flags(const QModelIndex &index) const;
#endif
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
}; // VocabularyModel

#endif // VOCABULARYMODEL_H