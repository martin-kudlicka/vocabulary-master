#ifndef FIELDSMODEL_H
#define FIELDSMODEL_H

#include <QtCore/QAbstractTableModel>
#include "vocabulary.h"

class FieldsModel : public QAbstractTableModel
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

    private:
        const Vocabulary *_vVocabulary;

        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole*/);
}; // FieldsModel

#endif // FIELDSMODEL_H