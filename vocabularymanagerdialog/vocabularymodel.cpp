#include "vocabularymanagerdialog/vocabularymodel.h"

int VocabularyModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant VocabularyModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case ColumnLang1:
                    return _vVocabulary->GetWord(_iCategoryId, index.row(), COLUMN_LANG1);
                case ColumnLang2:
                    return _vVocabulary->GetWord(_iCategoryId, index.row(), COLUMN_LANG2);
                case ColumnPriority:
                    return QVariant();
            } // switch
        default:
            return QVariant();
    } // switch
} // data

const void VocabularyModel::InsertRow(const int &pRow)
{
	beginInsertRows(QModelIndex(), pRow, pRow);
	_vVocabulary->AddWord(_iCategoryId);
	endInsertRows();
} // InsertRow

int VocabularyModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return _vVocabulary->GetWordCount(_iCategoryId);
} // rowCount

VocabularyModel::VocabularyModel(const Vocabulary *pVocabulary, const int &pCategoryId, QObject *pParent /* NULL */) : QAbstractTableModel(pParent)
{
    _vVocabulary = pVocabulary;
    _iCategoryId = pCategoryId;
} // VocabularyModel