#include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"

const void FieldsModel::AddRow()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _vVocabulary->AddField();
    endInsertRows();
} // AddRow

int FieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant FieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	int iFieldId = _vVocabulary->GetFieldId(index.row());

	switch (index.column()) {
		case ColumnTemplateName:
			switch (role) {
				case Qt::DisplayRole:
				case Qt::EditRole:
					return _vVocabulary->GetFieldTemplateName(iFieldId);
				default:
					return QVariant();
			} // switch
		case ColumnName:
			switch (role) {
				case Qt::DisplayRole:
				case Qt::EditRole:
					return _vVocabulary->GetFieldName(iFieldId);
				default:
					return QVariant();
			} // switch
        case ColumnSpeech:
            switch (role) {
				case Qt::CheckStateRole:
                    if (_vVocabulary->GetFieldAttributes(iFieldId) & Vocabulary::FieldAttributeSpeech) {
                        return Qt::Checked;
                    } else {
                        return Qt::Unchecked;
                    } // if else
				default:
					return QVariant();
			} // switch
		case ColumnLanguage:
			switch (role) {
				case Qt::DisplayRole:
					return _vVocabulary->GetLanguageName(_vVocabulary->GetFieldLanguage(iFieldId));
				case Qt::EditRole:
					return _vVocabulary->GetFieldLanguage(iFieldId);
				default:
					return QVariant();
			} // switch
		default:
			return QVariant();
	} // switch
} // data

FieldsModel::FieldsModel(const Vocabulary *pVocabulary, QObject *pParent /* NULL */) : QAbstractItemModel(pParent)
{
	_vVocabulary = pVocabulary;
} // FieldsModel

Qt::ItemFlags FieldsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags ifFlags = QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

    if (index.column() == ColumnSpeech) {
        ifFlags |= Qt::ItemIsUserCheckable;
    } // if

    return ifFlags;
} // flags

QVariant FieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (section) {
                case ColumnTemplateName:
                    return tr("Identifier");
                case ColumnName:
                    return tr("Name");
                case ColumnSpeech:
                    return tr("Speech");
                case ColumnLanguage:
                    return tr("Language");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex FieldsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
	return createIndex(row, column);
} // index

QModelIndex FieldsModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
} // parent

const void FieldsModel::RemoveRow(const int &pRow)
{
	beginRemoveRows(QModelIndex(), pRow, pRow);
	int iFieldId = _vVocabulary->GetFieldId(pRow);
	_vVocabulary->RemoveField(iFieldId);
	endRemoveRows();
} // RemoveRow

int FieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex()) {
        return _vVocabulary->GetFieldCount();
    } else {
        return 0;
    } // if
} // rowCount

bool FieldsModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole*/)
{
    if (role != Qt::EditRole && role != Qt::CheckStateRole) {
        return false;
    } // if

    int iFieldId = _vVocabulary->GetFieldId(index.row());

    switch (index.column()) {
        case ColumnTemplateName:
            _vVocabulary->SetFieldTemplateName(iFieldId, value.toString());
            break;
        case ColumnName:
            _vVocabulary->SetFieldName(iFieldId, value.toString());
            break;
        case ColumnSpeech:
            {
                Vocabulary::FieldAttributes faAttributes = _vVocabulary->GetFieldAttributes(iFieldId);
                faAttributes ^= Vocabulary::FieldAttributeSpeech;
                _vVocabulary->SetFieldAttributes(iFieldId, faAttributes);
            }
        case ColumnLanguage:
            _vVocabulary->SetFieldLanguage(iFieldId, static_cast<Vocabulary::eFieldLanguage>(value.toInt()));
    } // switch

    emit dataChanged(index, index);
    return true;
} // setData

const void FieldsModel::Swap(const int &pSourceRow, const int &pDestinationRow)
{
	int iSourceFieldId = _vVocabulary->GetFieldId(pSourceRow);
	int iDestinationFieldId = _vVocabulary->GetFieldId(pDestinationRow);
	_vVocabulary->SwapFields(iSourceFieldId, iDestinationFieldId);

	emit dataChanged(index(pSourceRow, 0), index(pSourceRow, ColumnCount - 1));
	emit dataChanged(index(pDestinationRow, 0), index(pDestinationRow, ColumnCount - 1));
} // Swap