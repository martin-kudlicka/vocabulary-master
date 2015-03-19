#ifndef VOCABULARYORGANIZERMODEL_H
#define VOCABULARYORGANIZERMODEL_H

#include <QtCore/QAbstractTableModel>

#include "vocabularyorganizer.h"

class VocabularyOrganizerModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
		enum Column
		{
			ColumnVocabularyFile,
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
			ColumnEnabled,
#endif
			ColumnCount
		}; // Column

		         VocabularyOrganizerModel(VocabularyOrganizer *organizer, QWidget *parent);
		virtual ~VocabularyOrganizerModel() override;

		void addRow   ();
		void removeRow(quint8 row);

	private:
		QWidget             *_parent;
		VocabularyOrganizer *_organizer;

		virtual int           columnCount(const QModelIndex &parent = QModelIndex())                            const override;
		virtual QVariant      data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const override;
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
		virtual Qt::ItemFlags flags      (const QModelIndex &index)                                             const override;
#endif
		virtual QVariant      headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual int           rowCount   (const QModelIndex &parent = QModelIndex())                            const override;
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
		virtual bool          setData    (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)   override;
#endif
}; // VocabularyOrganizerModel

#endif // VOCABULARYORGANIZERMODEL_H