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
#ifdef TRY
						return _voOrganizer->GetVocabularyInfo(index.row()).vVocabulary->GetName();
#else
						QString qsFile = _voOrganizer->GetVocabularyInfo(index.row()).sviVocabularyInfo.qsFile;
						return QDir::toNativeSeparators(qsFile);
#endif
					}
				default:
					return QVariant();
			} // switch
#if !defined(FREE) && !defined(TRY)
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
#endif
		default:
			return QVariant();
	} // switch
} // data

#if !defined(FREE) && !defined(TRY)
Qt::ItemFlags VocabularyOrganizerModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags ifFlags = QAbstractItemModel::flags(index);

	if (index.column() == ColumnEnabled) {
		ifFlags |= Qt::ItemIsUserCheckable;
	} // if

	return ifFlags;
} // flags
#endif

QVariant VocabularyOrganizerModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
	switch (role) {
		case Qt::DisplayRole:
			switch (section) {
				case ColumnVocabularyFile:
					return tr("Vocabulary");
#if !defined(FREE) && !defined(TRY)
				case ColumnEnabled:
					return tr("Enabled");
#endif
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

#if !defined(FREE) && !defined(TRY)
bool VocabularyOrganizerModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
	switch (index.column()) {
		case ColumnEnabled:
			_voOrganizer->SetVocabularyEnabled(index.row(), value.toBool(), _qwParent);

			emit dataChanged(index, index);
			return true;
		default:
			return false;
	} // switch
} // setData
#endif

VocabularyOrganizerModel::VocabularyOrganizerModel(VocabularyOrganizer *pOrganizer, QWidget *pParent)
{
	_voOrganizer = pOrganizer;
	_qwParent = pParent;
} // VocabularyOrganizerModel