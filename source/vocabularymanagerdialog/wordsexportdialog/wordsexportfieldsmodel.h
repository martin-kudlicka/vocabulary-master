#ifndef WORDSEXPORTFIELDSMODEL_H
#define WORDSEXPORTFIELDSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "vocabulary.h"

class WordsExportFieldsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
		enum Column
		{
			ColumnLanguage,
			ColumnName,
			ColumnMark,
			ColumnCount
		}; // Column

                 WordsExportFieldsModel(const Vocabulary *vocabulary, QObject *parent = NULL);
		virtual ~WordsExportFieldsModel();

		virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const;
		virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const;

    private:
        const Vocabulary *_vocabulary;

        virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const;
		virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const;
                quint8      fieldId    (const int &row)                                                       const;
        virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex parent     (const QModelIndex &index)                                             const;
}; // WordsExportFieldsModel

#endif // WORDSEXPORTFIELDSMODEL_H