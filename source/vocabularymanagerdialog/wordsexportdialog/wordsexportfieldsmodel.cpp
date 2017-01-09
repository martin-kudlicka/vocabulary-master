#include "vocabularymanagerdialog/wordsexportdialog/wordsexportfieldsmodel.h"

const QString TEMPLATE_MARK = "${%1}";

WordsExportFieldsModel::WordsExportFieldsModel(const Vocabulary *vocabulary, QObject *parent /* Q_NULLPTR */) : QAbstractItemModel(parent), _vocabulary(vocabulary)
{
}

WordsExportFieldsModel::~WordsExportFieldsModel()
{
}

QModelIndex WordsExportFieldsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
}

int WordsExportFieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex())
	{
        // ignore built-in fields
        quint8 count = 0;
        for (quint8 vocabularyFieldId : _vocabulary->fieldIds())
		{
            if (_vocabulary->fieldBuiltIn(vocabularyFieldId) == VocabularyDatabase::FieldBuiltIn::None)
			{
                count++;
            }
        }
        return count;
    }
	else
	{
        return 0;
    }
}

int WordsExportFieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
}

QVariant WordsExportFieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            switch (index.column())
			{
				case ColumnLanguage:
					{
						const quint8 languageFieldId                     = fieldId(index.row());
						const VocabularyDatabase::FieldLanguage language = _vocabulary->fieldLanguage(languageFieldId);
						return _vocabulary->languageName(language);
					}
                case ColumnName:
                    {
                        const quint8 languageFieldId = fieldId(index.row());
						return _vocabulary->fieldName(languageFieldId);
                    }
                case ColumnMark:
					const quint8 languageFieldId = fieldId(index.row());
                    const QString templateText   = _vocabulary->fieldTemplateName(languageFieldId);
                    return TEMPLATE_MARK.arg(templateText);
            }
        default:
            return QVariant();
    }
}

quint8 WordsExportFieldsModel::fieldId(const int &row) const
{
    quint8 num = 0;
    for (quint8 vocabularyFieldId : _vocabulary->fieldIds())
	{
        if (_vocabulary->fieldBuiltIn(vocabularyFieldId) == VocabularyDatabase::FieldBuiltIn::None)
		{
            if (row == num)
			{
                return vocabularyFieldId;
            }
			else
			{
                num++;
            }
        }
    }

    return VocabularyDatabase::NOT_FOUND;
}

QVariant WordsExportFieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            switch (section)
			{
				case ColumnLanguage:
					return tr("Language");
                case ColumnName:
                    return tr("Name");
				case ColumnMark:
					return tr("Mark");
            }
        default:
            return QVariant();
    }
}

QModelIndex WordsExportFieldsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}