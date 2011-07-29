#include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"

FieldsModel::FieldsModel(const Vocabulary *pVocabulary, QObject *pParent /* NULL */)
{
    _vVocabulary = pVocabulary;
} // FieldsModel

int FieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

Qt::ItemFlags FieldsModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
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
                case ColumnLanguage:
                    return tr("Language");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

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
    if (role != Qt::EditRole) {
        return false;
    } // if

    switch (index.column()) {
        case ColumnTemplateName:
            _vVocabulary->SetFieldTemplateName(index.row() + 1, value.toString());
            break;
        case ColumnName:
            _vVocabulary->SetFieldName(index.row() + 1, value.toString());
            break;
        case ColumnLanguage:
            _vVocabulary->SetFieldLanguage(index.row() + 1, static_cast<Vocabulary::eFieldLanguage>(value.toInt()));
    } // switch

    emit dataChanged(index, index);
    return true;
} // setData