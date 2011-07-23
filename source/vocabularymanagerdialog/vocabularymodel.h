#ifndef VOCABULARYMODEL_H
#define VOCABULARYMODEL_H

#include <QtCore/QAbstractTableModel>
#include "vocabulary.h"

class VocabularyModel : public QAbstractTableModel
{
	Q_OBJECT

    public:
        VocabularyModel(const Vocabulary *pVocabulary, const int &pCategoryId, QObject *pParent = NULL);

		const void InsertRow(const int &pRow);
        const void RemoveRow(const int &pRow);
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
		bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    private:
        int _iCategoryId;
        const Vocabulary *_vVocabulary;

        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
}; // VocabularyModel

#endif // VOCABULARYMODEL_H