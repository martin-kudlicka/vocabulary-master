#include "vocabularymanagerdialog/vocabularymodel.h"

const void VocabularyModel::AddRow()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _vVocabulary->AddRecord(_iCategoryId);
    endInsertRows();
} // AddRow

int VocabularyModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return _vVocabulary->GetFieldCount();
} // columnCount

QVariant VocabularyModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            return _vVocabulary->GetDataText(_iCategoryId, index.row(), _vVocabulary->GetFieldId(index.column()));
        default:
            return QVariant();
    } // switch
} // data

QVariant VocabularyModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            if (orientation == Qt::Horizontal) {
                return _vVocabulary->GetFieldName(_vVocabulary->GetFieldId(section));
            } else {
                return section + 1;
            } // if else
        default:
            return QVariant();
    } // switch
} // headerData

const void VocabularyModel::RemoveRow(const int &pRow)
{
    beginRemoveRows(QModelIndex(), pRow, pRow);
    _vVocabulary->RemoveRecord(_iCategoryId, pRow);
    endRemoveRows();
} // RemoveRow

int VocabularyModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return _vVocabulary->GetRecordCount(_iCategoryId);
} // rowCount

bool VocabularyModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
	if (role != Qt::EditRole) {
		return false;
	} // if

	int iFieldId = _vVocabulary->GetFieldId(index.column());
	_vVocabulary->SetDataText(_iCategoryId, index.row(), iFieldId, value.toString());

	emit dataChanged(index, index);
	return true;
} // setData

VocabularyModel::VocabularyModel(Vocabulary *pVocabulary, const int &pCategoryId, QObject *pParent /* NULL */) : QAbstractTableModel(pParent)
{
    _vVocabulary = pVocabulary;
    _iCategoryId = pCategoryId;
} // VocabularyModel