#ifndef FIELDSMODEL_H
#define FIELDSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "vocabulary.h"

class FieldsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        enum Column
		{
            ColumnTemplateName,
            ColumnName,
            ColumnSpeech,
            ColumnShow,
            ColumnLanguage,
            ColumnCount
        }; // Column

                 FieldsModel(Vocabulary *vocabulary, QObject *parent = NULL);
		virtual ~FieldsModel() override;

                void        addRow   ();
		virtual QVariant    data     (const QModelIndex &index, int role = Qt::DisplayRole)           const override;
		virtual QModelIndex index    (int row, int column, const QModelIndex &parent = QModelIndex()) const override;
		        void        removeRow(quint8 row);
        virtual int         rowCount (const QModelIndex &parent = QModelIndex())                      const override;
		        void        swap     (quint8 sourceRow, quint8 destinationRow);

    private:
        Vocabulary *_vocabulary;

        virtual int           columnCount(const QModelIndex &parent = QModelIndex())                            const override;
        virtual Qt::ItemFlags flags      (const QModelIndex &index)                                             const override;
        virtual QVariant      headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual QModelIndex   parent     (const QModelIndex &index)                                             const override;
        virtual bool          setData    (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)   override;
}; // FieldsModel

#endif // FIELDSMODEL_H