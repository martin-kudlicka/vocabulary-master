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

                 WordsExportFieldsModel(const Vocabulary *vocabulary, QObject *parent = nullptr);
		virtual ~WordsExportFieldsModel() override;

		virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const override;
		virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const override;

    private:
        const Vocabulary *_vocabulary;

        virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const override;
		virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const override;
                quint8      fieldId    (const int &row)                                                       const;
        virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        virtual QModelIndex parent     (const QModelIndex &index)                                             const override;
}; // WordsExportFieldsModel

#endif // WORDSEXPORTFIELDSMODEL_H