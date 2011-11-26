#ifndef VOCABULARYORGANIZERMODEL_H
#define VOCABULARYORGANIZERMODEL_H

#include <QtCore/QAbstractTableModel>

#include "vocabularyorganizer.h"

class VocabularyOrganizerModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
		enum eColumn {
			ColumnVocabularyFile,
#if !defined(FREE) && !defined(TRY)
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
#if !defined(FREE) && !defined(TRY)
		virtual Qt::ItemFlags flags(const QModelIndex &index) const;
#endif
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
#if !defined(FREE) && !defined(TRY)
		virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
#endif
}; // VocabularyOrganizerModel

#endif // VOCABULARYORGANIZERMODEL_H