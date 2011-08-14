#include "vocabularymanagerdialog/wordsimportdialog/wordsimportfieldsmodel.h"

WordsImportFieldsModel::WordsImportFieldsModel(const Vocabulary *pVocabulary, QObject *pParent /* NULL */)
{
    _vVocabulary = pVocabulary;
} // WordsImportFieldsModel

int WordsImportFieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant WordsImportFieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case ColumnName:
                    {
                        int iFieldId = _vVocabulary->GetFieldId(index.row());
                        return _vVocabulary->GetFieldName(iFieldId);
                    }
				case ColumnLanguage:
					{
						int iFieldId = _vVocabulary->GetFieldId(index.row());
						Vocabulary::eFieldLanguage eflLanguage = _vVocabulary->GetFieldLanguage(iFieldId);
						return _vVocabulary->GetLanguageName(eflLanguage);
					}
                default:
                    return QVariant();
            } // switch
        default:
            return QVariant();
    } // switch
} // data

QVariant WordsImportFieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (section) {
                case ColumnName:
                    return tr("Name");
				case ColumnLanguage:
					return tr("Language");
                case ColumnEditor:
                    return tr("Pattern");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex WordsImportFieldsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
} // index

QModelIndex WordsImportFieldsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent

int WordsImportFieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex()) {
        return _vVocabulary->GetFieldCount();
    } else {
        return 0;
    } // if else
} // rowCount