#ifndef FIELDSMODEL_H
#define FIELDSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "vocabulary.h"

class FieldsModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_ENUMS(eColumn)

    public:
        enum eColumn {
            ColumnTemplateName,
            ColumnName,
            ColumnLanguage,
            ColumnCount
        }; // eColumn

        FieldsModel(const Vocabulary *pVocabulary, QObject *pParent = NULL);

        const void AddRow();
		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		const void RemoveRow(const int &pRow);
		const void Swap(const int &pSourceRow, const int &pDestinationRow);

    private:
        const Vocabulary *_vVocabulary;

        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		QModelIndex parent(const QModelIndex &index) const;
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole*/);
}; // FieldsModel

#endif // FIELDSMODEL_H