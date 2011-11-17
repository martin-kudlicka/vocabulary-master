#include "vocabularyorganizerdialog/vocabularyorganizermodel.h"

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
				return vVocabulary->GetVocabularyFile();
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