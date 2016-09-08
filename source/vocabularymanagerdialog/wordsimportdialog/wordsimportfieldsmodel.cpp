#include "vocabularymanagerdialog/wordsimportdialog/wordsimportfieldsmodel.h"

#include "vocabularymanagerdialog/prioritydelegate.h"

WordsImportFieldsModel::WordsImportFieldsModel(const Vocabulary *vocabulary, QObject *parent /* nullptr */) : QAbstractItemModel(parent), _vocabulary(vocabulary)
{
	for (quint8 fieldIndex = 0; fieldIndex < _vocabulary->fieldCount(); fieldIndex++)
	{
		_editorData.append(QString());
	}
}

WordsImportFieldsModel::~WordsImportFieldsModel()
{
}

QVariant WordsImportFieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	switch (index.column())
	{
		case ColumnLanguage:
			switch (role)
			{
				case Qt::DisplayRole:
					{
						const quint8 fieldId                             = _vocabulary->fieldId(index.row());
						const VocabularyDatabase::FieldLanguage language = _vocabulary->fieldLanguage(fieldId);
						return _vocabulary->languageName(language);
					}
				default:
					return QVariant();
			}
		case ColumnName:
			switch (role)
			{
				case Qt::DisplayRole:
					{
						const quint8 fieldId = _vocabulary->fieldId(index.row());
						return _vocabulary->fieldName(fieldId);
					}
				default:
					return QVariant();
			}
		case ColumnEditor:
			{
				const quint8 fieldId = _vocabulary->fieldId(index.row());

				if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
				{
					const VocabularyDatabase::FieldBuiltIn builtIn = _vocabulary->fieldBuiltIn(fieldId);
					switch (builtIn)
					{
						case VocabularyDatabase::FieldBuiltInEnabled:
							switch (role)
							{
								case Qt::EditRole:
								case Qt::CheckStateRole:
									if (_editorData.at(index.row()).isEmpty())
									{
										return Qt::Checked;
									}
									else
									{
										return _editorData.at(index.row()).toUInt();
									}
								default:
									return QVariant();
							}
						case VocabularyDatabase::FieldBuiltInPriority:
							switch (role)
							{
								case Qt::EditRole:
									if (_editorData.at(index.row()).isEmpty())
									{
										return PriorityDelegate::RECORD_PRIORITY_MIN;
									}
									else
									{
										return _editorData.at(index.row()).toUInt();
									}
								default:
									return QVariant();
							}
					}
				}
				else
				{
					switch (role)
					{
						case Qt::EditRole:
							return _editorData.at(index.row());
						default:
							return QVariant();
					}
				}
			}
		default:
			return QVariant();
	}
}

QModelIndex WordsImportFieldsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
}

int WordsImportFieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex())
	{
        return _vocabulary->fieldCount();
    }
	else
	{
        return 0;
    }
}

int WordsImportFieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
}

Qt::ItemFlags WordsImportFieldsModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags itemFlags = QAbstractItemModel::flags(index);

	if (index.column() == ColumnEditor)
	{
		const quint8 fieldId                          = _vocabulary->fieldId(index.row());
		const VocabularyDatabase::FieldType fieldType = _vocabulary->fieldType(fieldId);
		if (fieldType == VocabularyDatabase::FieldTypeCheckBox)
		{
			itemFlags |= Qt::ItemIsUserCheckable;
		}
	}

	return itemFlags;
}

QVariant WordsImportFieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
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
                case ColumnEditor:
                    return tr("Pattern");
            }
        default:
            return QVariant();
    }
}

QModelIndex WordsImportFieldsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

bool WordsImportFieldsModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
	_editorData[index.row()] = value.toString();

	emit dataChanged(index, index);
	return true;
}