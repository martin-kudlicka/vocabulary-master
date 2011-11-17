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
	switch (role) {
		case Qt::DisplayRole:
			{
				const Vocabulary *vVocabulary = _voOrganizer->GetVocabularyInfo(index.row());
				return QDir::toNativeSeparators(vVocabulary->GetVocabularyFile());
			}
		default:
			return QVariant();
	} // switch
} // data

QVariant VocabularyOrganizerModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
	switch (role) {
		case Qt::DisplayRole:
			return tr("Vocabulary");
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