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
	int iFieldId = _vVocabulary->GetFieldId(index.column());
	VocabularyDatabase::FieldAttributes faAttributes = _vVocabulary->GetFieldAttributes(iFieldId);

	if (faAttributes & VocabularyDatabase::FieldAttributeBuiltIn) {
		VocabularyDatabase::eFieldBuiltIn efbBuiltIn = _vVocabulary->GetFieldBuiltIn(iFieldId);
		if (efbBuiltIn == VocabularyDatabase::FieldBuiltInEnabled) {
			switch (role) {
				case Qt::CheckStateRole:
					return _vVocabulary->GetDataText(_iCategoryId, index.row(), iFieldId);
				default:
					return QVariant();
			} // switch
		} // if
	} else {
		switch (role) {
			case Qt::DisplayRole:
				return _vVocabulary->GetDataText(_iCategoryId, index.row(), iFieldId);
			default:
				return QVariant();
		} // switch
	} // if else

	return QVariant();
} // data

Qt::ItemFlags VocabularyModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags ifFlags = QAbstractItemModel::flags(index);

	int iFieldId = _vVocabulary->GetFieldId(index.column());
	VocabularyDatabase::FieldAttributes faAttributes = _vVocabulary->GetFieldAttributes(iFieldId);
	if (faAttributes & VocabularyDatabase::FieldAttributeBuiltIn) {
		VocabularyDatabase::eFieldBuiltIn efbBuiltIn = _vVocabulary->GetFieldBuiltIn(iFieldId);
		if (efbBuiltIn == VocabularyDatabase::FieldBuiltInEnabled) {
			ifFlags |= Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
		} // if
	} // if

	return ifFlags;
} // flags

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