#include "vocabularyorganizerdialog/vocabularyorganizermodel.h"

#ifndef EDITION_TRY
# include <QtCore/QDir>
#endif

VocabularyOrganizerModel::VocabularyOrganizerModel(VocabularyOrganizer *organizer, QWidget *parent) : _organizer(organizer), _parent(parent)
{
}

VocabularyOrganizerModel::~VocabularyOrganizerModel()
{
}

void VocabularyOrganizerModel::addRow()
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	endInsertRows();
}

void VocabularyOrganizerModel::removeRow(quint8 row)
{
	beginRemoveRows(QModelIndex(), row, row);
	endRemoveRows();
}

int VocabularyOrganizerModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
	return ColumnCount;
}

QVariant VocabularyOrganizerModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	switch (index.column())
	{
		case ColumnVocabularyFile:
			switch (role)
			{
				case Qt::DisplayRole:
					{
#ifdef EDITION_TRY
						return _organizer->vocabularyInfo(index.row()).vocabulary->name();
#else
						const QString file = _organizer->vocabularyInfo(index.row()).vocabularyInfo.filePath;
						return QDir::toNativeSeparators(file);
#endif
					}
				default:
					return QVariant();
			}
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
		case ColumnEnabled:
			switch (role)
			{
				case Qt::CheckStateRole:
					{
						const bool enabled = _organizer->vocabularyInfo(index.row()).vocabularyInfo.enabled;
						if (enabled)
						{
							return Qt::Checked;
						}
						else
						{
							return Qt::Unchecked;
						}
					}
				default:
					return QVariant();
			}
#endif
		default:
			return QVariant();
	}
}

#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
Qt::ItemFlags VocabularyOrganizerModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags itemFlags = QAbstractItemModel::flags(index);

	if (index.column() == ColumnEnabled)
	{
		itemFlags |= Qt::ItemIsUserCheckable;
	}

	return itemFlags;
}
#endif

QVariant VocabularyOrganizerModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
	switch (role)
	{
		case Qt::DisplayRole:
			switch (section)
			{
				case ColumnVocabularyFile:
					return tr("Vocabulary");
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
				case ColumnEnabled:
					return tr("Enabled");
#endif
			}
		default:
			return QVariant();
	}
}

int VocabularyOrganizerModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
	if (parent == QModelIndex())
	{
		return _organizer->vocabularyCount();
	}
	else
	{
		return 0;
	}
}

#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
bool VocabularyOrganizerModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
	switch (index.column())
	{
		case ColumnEnabled:
			_organizer->setVocabularyEnabled(index.row(), value.toBool(), _parent);

			emit dataChanged(index, index);
			return true;
		default:
			return false;
	}
}
#endif