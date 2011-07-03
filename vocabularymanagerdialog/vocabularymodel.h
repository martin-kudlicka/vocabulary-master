#ifndef VOCABULARYMODEL_H
#define VOCABULARYMODEL_H

#include <QtCore/QAbstractTableModel>
#include "vocabulary.h"

class VocabularyModel : public QAbstractTableModel
{
    Q_ENUMS(eColumn)

    public:
		enum eColumn {
			ColumnLang1,
			ColumnLang2,
			ColumnCount
		}; // eColumn

        VocabularyModel(const Vocabulary *pVocabulary, const int &pCategoryId, QObject *pParent = NULL);

		const void InsertRow(const int &pRow);
        int rowCount(const QModelIndex &parent = QModelIndex()) const;

    private:
        int _iCategoryId;
        const Vocabulary *_vVocabulary;

        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
}; // VocabularyModel

#endif // VOCABULARYMODEL_H