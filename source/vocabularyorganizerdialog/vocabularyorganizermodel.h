#ifndef VOCABULARYORGANIZERMODEL_H
#define VOCABULARYORGANIZERMODEL_H

#include <QtCore/QAbstractTableModel>

#include "vocabularyorganizer.h"

class VocabularyOrganizerModel : public QAbstractTableModel
{
	public:
		enum eColumn {
			ColumnVocabularyFile,
#ifndef FREE
			ColumnEnabled,
#endif
			ColumnCount
		}; // eColumn

		VocabularyOrganizerModel(VocabularyOrganizer *pOrganizer, QWidget *pParent);

		const void AddRow();
		const void RemoveRow(const int &pRow);

	private:
		QWidget *_qwParent;
		VocabularyOrganizer *_voOrganizer;

		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
#ifndef FREE
		virtual Qt::ItemFlags flags(const QModelIndex &index) const;
#endif
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
#ifndef FREE
		virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
#endif
}; // VocabularyOrganizerModel

#endif // VOCABULARYORGANIZERMODEL_H