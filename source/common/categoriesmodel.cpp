#include "vocabularymanagerdialog/wordsimportdialog/categoriesmodel.h"

CategoriesModel::CategoriesModel(const Vocabulary *pVocabulary, QObject *pParent /* NULL */)
{
    _vVocabulary = pVocabulary;
} // CategoriesModel

int CategoriesModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant CategoriesModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            {
                int iCategoryId = _vVocabulary->GetCategoryId(index.row());
                return _vVocabulary->GetCategoryName(iCategoryId);
            }
        default:
            return QVariant();
    } // switch
} // data

QVariant CategoriesModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            return tr("Category");
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex CategoriesModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
} // index

QModelIndex CategoriesModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent

int CategoriesModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex()) {
        return _vVocabulary->GetCategoryCount();
    } else {
        return 0;
    } // if else
} // rowCount