#include "vocabularymanagerdialog/vocabularymodel.h"

#ifndef EDITION_FREE
# include "vocabularymanagerdialog/prioritydelegate.h"
#endif

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

	switch (_vVocabulary->GetFieldType(iFieldId)) {
		case VocabularyDatabase::FieldTypeLineEdit:
			switch (role) {
				case Qt::DisplayRole:
					return _vVocabulary->GetDataText(_iCategoryId, index.row(), iFieldId);
				default:
					return QVariant();
			} // switch
#ifndef EDITION_FREE
		case VocabularyDatabase::FieldTypeCheckBox:
			switch (role) {
				case Qt::CheckStateRole:
					{
						QString qsChecked = _vVocabulary->GetDataText(_iCategoryId, index.row(), iFieldId);
						if (qsChecked.isEmpty()) {
							return Qt::Checked;
						} else {
							return qsChecked.toInt();
						} // if else
					}
				default:
					return QVariant();
			} // switch
		case VocabularyDatabase::FieldTypeSpinBox:
			switch (role) {
				case Qt::DisplayRole:
					{
						QString qsPriority = _vVocabulary->GetDataText(_iCategoryId, index.row(), iFieldId);
						if (qsPriority.isEmpty()) {
							return PriorityDelegate::RECORD_PRIORITY_MIN;
						} else {
							return qsPriority.toInt();
						} // if else
					}
				default:
					return QVariant();
			} // switch
#endif
	} // switch

	return QVariant();
} // data

#ifndef EDITION_FREE
Qt::ItemFlags VocabularyModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags ifFlags = QAbstractItemModel::flags(index);

	int iFieldId = _vVocabulary->GetFieldId(index.column());
	if (_vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
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