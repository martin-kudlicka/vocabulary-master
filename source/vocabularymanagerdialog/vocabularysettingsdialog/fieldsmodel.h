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
            ColumnSpeech,
            ColumnLanguage,
            ColumnCount
        }; // eColumn

        FieldsModel(const Vocabulary *pVocabulary, QObject *pParent = NULL);

        const void AddRow();
		virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		const void RemoveRow(const int &pRow);
		const void Swap(const int &pSourceRow, const int &pDestinationRow);

    private:
        const Vocabulary *_vVocabulary;

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		virtual QModelIndex parent(const QModelIndex &index) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole*/);
}; // FieldsModel

#endif // FIELDSMODEL_H