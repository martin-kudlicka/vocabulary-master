#ifndef WORDSEXPORTFIELDSMODEL_H
#define WORDSEXPORTFIELDSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "vocabulary.h"

class WordsExportFieldsModel : public QAbstractItemModel
{
	Q_ENUMS(eColumn)

    public:
		enum eColumn {
			ColumnLanguage,
			ColumnName,
			ColumnMark,
			ColumnCount
		}; // eColumn

        WordsExportFieldsModel(const Vocabulary *pVocabulary, QObject *pParent = NULL);

		virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    private:
        const Vocabulary *_vVocabulary;

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex parent(const QModelIndex &index) const;
}; // WordsExportFieldsModel

#endif // WORDSEXPORTFIELDSMODEL_H