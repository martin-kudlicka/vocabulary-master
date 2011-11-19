#include "vocabularyorganizerdialog/vocabularyorganizermodel.h"

#include <QtCore/QDir>

const void VocabularyOrganizerModel::AddRow()
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	endInsertRows();
} // AddRow

int VocabularyOrganizerModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
	return ColumnCount;
} // columnCount

QVariant VocabularyOrganizerModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	switch (index.column()) {
		case ColumnVocabularyFile:
			switch (role) {
				case Qt::DisplayRole:
					{
						QString qsFile = _voOrganizer->GetVocabularyInfo(index.row()).sviVocabularyInfo.qsFile;
						return QDir::toNativeSeparators(qsFile);
					}
				default:
					return QVariant();
			} // switch
		case ColumnEnabled:
			switch (role) {
				case Qt::CheckStateRole:
					{
						bool bEnabled = _voOrganizer->GetVocabularyInfo(index.row()).sviVocabularyInfo.bEnabled;
						if (bEnabled) {
							return Qt::Checked;
						} else {
							return Qt::Unchecked;
						} // if else
					}
				default:
					return QVariant();
			} // switch
		default:
			return QVariant();
	} // switch
} // data

QVariant VocabularyOrganizerModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
	switch (role) {
		case Qt::DisplayRole:
			switch (section) {
				case ColumnVocabularyFile:
					return tr("Vocabulary");
				case ColumnEnabled:
					return tr("Enabled");
			} // switch
		default:
			return QVariant();
	} // switch
} // headerData

const void VocabularyOrganizerModel::RemoveRow(const int &pRow)
{
	beginRemoveRows(QModelIndex(), pRow, pRow);
	endRemoveRows();
} // RemoveRow

int VocabularyOrganizerModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
	if (parent == QModelIndex()) {
		return _voOrganizer->GetVocabularyCount();
	} else {
		return 0;
	} // if else
} // rowCount

VocabularyOrganizerModel::VocabularyOrganizerModel(const VocabularyOrganizer *pOrganizer)
{
	_voOrganizer = pOrganizer;
} // VocabularyOrganizerModel