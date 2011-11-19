#ifndef VOCABULARYORGANIZERMODEL_H
#define VOCABULARYORGANIZERMODEL_H

#include <QtCore/QAbstractTableModel>

#include "vocabularyorganizer.h"

class VocabularyOrganizerModel : public QAbstractTableModel
{
	public:
		VocabularyOrganizerModel(const VocabularyOrganizer *pOrganizer);

		const void AddRow();
		const void RemoveRow(const int &pRow);

	private:
		enum eColumn {
			ColumnVocabularyFile,
#ifndef FREE
			ColumnEnabled,
#endif
			ColumnCount
		}; // eColumn

		const VocabularyOrganizer *_voOrganizer;

		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
}; // VocabularyOrganizerModel

#endif // VOCABULARYORGANIZERMODEL_H