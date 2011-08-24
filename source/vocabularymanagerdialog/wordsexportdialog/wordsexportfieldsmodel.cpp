#include "vocabularymanagerdialog/wordsexportdialog/wordsexportfieldsmodel.h"

const QString TEMPLATE_MARK = "${%1}";

WordsExportFieldsModel::WordsExportFieldsModel(const Vocabulary *pVocabulary, QObject *pParent /* NULL */) : QAbstractItemModel(pParent)
{
    _vVocabulary = pVocabulary;
} // WordsExportFieldsModel

int WordsExportFieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant WordsExportFieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
				case ColumnLanguage:
					{
						int iFieldId = _vVocabulary->GetFieldId(index.row());
						Vocabulary::eFieldLanguage eflLanguage = _vVocabulary->GetFieldLanguage(iFieldId);
						return _vVocabulary->GetLanguageName(eflLanguage);
					}
                case ColumnName:
                    {
                        int iFieldId = _vVocabulary->GetFieldId(index.row());
                        return _vVocabulary->GetFieldName(iFieldId);
                    }
                case ColumnMark:
                    int iFieldId = _vVocabulary->GetFieldId(index.row());
                    QString qsTemplate = _vVocabulary->GetFieldTemplateName(iFieldId);
                    return TEMPLATE_MARK.arg(qsTemplate);
            } // switch
        default:
            return QVariant();
    } // switch
} // data

QVariant WordsExportFieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (section) {
				case ColumnLanguage:
					return tr("Language");
                case ColumnName:
                    return tr("Name");
				case ColumnMark:
					return tr("Mark");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex WordsExportFieldsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
} // index

QModelIndex WordsExportFieldsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent

int WordsExportFieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex()) {
        return _vVocabulary->GetFieldCount();
    } else {
        return 0;
    } // if else
} // rowCount