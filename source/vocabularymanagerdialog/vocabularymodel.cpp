#include "vocabularymanagerdialog/vocabularymodel.h"

const QString COLUMN_NOTE = QT_TRANSLATE_NOOP("VocabularyModel", "Note (%1)");

int VocabularyModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant VocabularyModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch (index.column()) {
                case ColumnLang1:
                    return _vVocabulary->GetWord(_iCategoryId, index.row(), COLUMN_LANG1);
                case ColumnNote1:
                    return _vVocabulary->GetNote(_iCategoryId, index.row(), COLUMN_NOTE1);
                case ColumnLang2:
                    return _vVocabulary->GetWord(_iCategoryId, index.row(), COLUMN_LANG2);
                case ColumnNote2:
                    return _vVocabulary->GetNote(_iCategoryId, index.row(), COLUMN_NOTE2);
            } // switch
        default:
            return QVariant();
    } // switch
} // data

Qt::ItemFlags VocabularyModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags ifFlags;

	if (index.column() == ColumnLang1 || index.column() == ColumnNote1 || index.column() == ColumnLang2 || index.column() == ColumnNote2) {
		ifFlags = Qt::ItemIsEditable;
	} // if

	return ifFlags | QAbstractTableModel::flags(index);
} // flags

QVariant VocabularyModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            if (orientation == Qt::Horizontal) {
                switch (section) {
                    case ColumnLang1:
                        return _vVocabulary->GetSettings(KEY_LANGUAGE1);
                    case ColumnNote1:
                        return COLUMN_NOTE.arg(_vVocabulary->GetSettings(KEY_LANGUAGE1).toLower());
                    case ColumnLang2:
                        return _vVocabulary->GetSettings(KEY_LANGUAGE2);
                    case ColumnNote2:
                        return COLUMN_NOTE.arg(_vVocabulary->GetSettings(KEY_LANGUAGE2).toLower());
                } // switch
            } else {
                return section + 1;
            } // if else
        default:
            return QVariant();
    } // switch
} // headerData

const void VocabularyModel::InsertRow(const int &pRow)
{
	beginInsertRows(QModelIndex(), pRow, pRow);
	_vVocabulary->AddWord(_iCategoryId);
	endInsertRows();
} // InsertRow

const void VocabularyModel::RemoveRow(const int &pRow)
{
    beginRemoveRows(QModelIndex(), pRow, pRow);
    _vVocabulary->RemoveWord(_iCategoryId, pRow);
    endRemoveRows();
} // RemoveRow

int VocabularyModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return _vVocabulary->GetWordCount(_iCategoryId);
} // rowCount

bool VocabularyModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
	if (role != Qt::EditRole) {
		return false;
	} // if

	switch (index.column()) {
		case ColumnLang1:
			_vVocabulary->SetWord(value.toString(), _iCategoryId, index.row(), COLUMN_LANG1);
			break;
		case ColumnNote1:
			_vVocabulary->SetNote(value.toString(), _iCategoryId, index.row(), COLUMN_NOTE1);
			break;
		case ColumnLang2:
			_vVocabulary->SetWord(value.toString(), _iCategoryId, index.row(), COLUMN_LANG2);
			break;
		case ColumnNote2:
			_vVocabulary->SetNote(value.toString(), _iCategoryId, index.row(), COLUMN_NOTE2);
			break;
	} // switch

	emit dataChanged(index, index);
	return true;
} // setData

VocabularyModel::VocabularyModel(const Vocabulary *pVocabulary, const int &pCategoryId, QObject *pParent /* NULL */) : QAbstractTableModel(pParent)
{
    _vVocabulary = pVocabulary;
    _iCategoryId = pCategoryId;
} // VocabularyModel