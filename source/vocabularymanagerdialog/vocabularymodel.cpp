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
#ifndef FREE
	VocabularyDatabase::qfFieldAttributes qfaAttributes = _vVocabulary->GetFieldAttributes(iFieldId);

	if (qfaAttributes & VocabularyDatabase::FieldAttributeBuiltIn) {
		VocabularyDatabase::eFieldBuiltIn efbBuiltIn = _vVocabulary->GetFieldBuiltIn(iFieldId);
		switch (efbBuiltIn) {
			case VocabularyDatabase::FieldBuiltInEnabled:
				switch (role) {
					case Qt::CheckStateRole:
						{
							QString qsChecked = _vVocabulary->GetDataText(_iCategoryId, index.row(), iFieldId);
							if (qsChecked.isEmpty()) {
								return Qt::Checked;
							} else {
								return qsChecked;
							} // if
						}
					default:
						return QVariant();
				} // switch
			case VocabularyDatabase::FieldBuiltInPriority:
				switch (role) {
					case Qt::DisplayRole:
					case Qt::EditRole:
						return _vVocabulary->GetDataText(_iCategoryId, index.row(), iFieldId).toInt();
					default:
						return QVariant();
				} // switch
		} // switch
	} else {
#endif
		switch (role) {
			case Qt::DisplayRole:
				return _vVocabulary->GetDataText(_iCategoryId, index.row(), iFieldId);
			default:
				return QVariant();
		} // switch
#ifndef FREE
	} // if else
#endif

	return QVariant();
} // data

#ifndef FREE
Qt::ItemFlags VocabularyModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags ifFlags = QAbstractItemModel::flags(index);

	int iFieldId = _vVocabulary->GetFieldId(index.column());
	VocabularyDatabase::qfFieldAttributes qfaAttributes = _vVocabulary->GetFieldAttributes(iFieldId);
	if (qfaAttributes & VocabularyDatabase::FieldAttributeBuiltIn) {
		VocabularyDatabase::eFieldBuiltIn efbBuiltIn = _vVocabulary->GetFieldBuiltIn(iFieldId);
		switch (efbBuiltIn) {
			case VocabularyDatabase::FieldBuiltInEnabled:
				ifFlags |= Qt::ItemIsUserCheckable;
				break;
			case VocabularyDatabase::FieldBuiltInPriority:
				ifFlags |= Qt::ItemIsEditable;
		} // switch
	} // if

	return ifFlags;
} // flags
#endif

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