#ifndef WORDSIMPORTFIELDSMODEL_H
#define WORDSIMPORTFIELDSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "vocabulary.h"
#include <QtCore/QStringList>

class WordsImportFieldsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        enum Column
		{
			ColumnLanguage,
            ColumnName,
            ColumnEditor,
            ColumnCount
        }; // Column

                 WordsImportFieldsModel(const Vocabulary *vocabulary, QObject *parent = NULL);
		virtual ~WordsImportFieldsModel() override;

		virtual QVariant    data    (const QModelIndex &index, int role = Qt::DisplayRole)           const override;
        virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const override;

    private:
		      QStringList _editorData;
        const Vocabulary *_vocabulary;

        virtual int           columnCount(const QModelIndex &parent = QModelIndex())                            const override;
		virtual Qt::ItemFlags flags      (const QModelIndex &index)                                             const override;
        virtual QVariant      headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        virtual QModelIndex   parent     (const QModelIndex &index)                                             const override;
		virtual bool          setData    (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)   override;
}; // WordsImportFieldsModel

#endif // WORDSIMPORTFIELDSMODEL_H