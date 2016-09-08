#include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"

FieldsModel::FieldsModel(Vocabulary *vocabulary, QObject *parent /* nullptr */) : QAbstractItemModel(parent), _vocabulary(vocabulary)
{
}

FieldsModel::~FieldsModel()
{
}

void FieldsModel::addRow()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _vocabulary->addField();
    endInsertRows();
}

QVariant FieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	const qint8 fieldId = _vocabulary->fieldId(index.row());

	switch (index.column())
	{
		case ColumnTemplateName:
			switch (role)
			{
				case Qt::DisplayRole:
					return _vocabulary->fieldTemplateName(fieldId);
				default:
					return QVariant();
			}
		case ColumnName:
			switch (role)
			{
				case Qt::DisplayRole:
					return _vocabulary->fieldName(fieldId);
				default:
					return QVariant();
			}
        case ColumnSpeech:
            switch (role)
			{
				case Qt::CheckStateRole:
                    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeSpeech))
					{
                        return Qt::Checked;
                    }
					else
					{
                        return Qt::Unchecked;
                    }
				default:
					return QVariant();
			}
        case ColumnShow:
            switch (role)
			{
                case Qt::CheckStateRole:
                    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeShow))
					{
                        return Qt::Checked;
                    }
					else
					{
                        return Qt::Unchecked;
                    }
				default:
					return QVariant();
			}
		case ColumnLanguage:
			switch (role)
			{
				case Qt::EditRole:
					return _vocabulary->fieldLanguage(fieldId);
				default:
					return QVariant();
			}
		default:
			return QVariant();
	}
}

QModelIndex FieldsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
	return createIndex(row, column);
}

void FieldsModel::removeRow(quint8 row)
{
	beginRemoveRows(QModelIndex(), row, row);
	const qint8 fieldId = _vocabulary->fieldId(row);
	_vocabulary->removeField(fieldId);
	endRemoveRows();
}

int FieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
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

void FieldsModel::swap(quint8 sourceRow, quint8 destinationRow)
{
	const qint8 sourceFieldId      = _vocabulary->fieldId(sourceRow);
	const qint8 destinationFieldId = _vocabulary->fieldId(destinationRow);
	_vocabulary->swapFields(sourceFieldId, destinationFieldId);

	emit dataChanged(index(sourceRow, 0), index(sourceRow, ColumnCount - 1));
	emit dataChanged(index(destinationRow, 0), index(destinationRow, ColumnCount - 1));
}

int FieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
	return ColumnCount;
}

Qt::ItemFlags FieldsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags itemFlags = QAbstractItemModel::flags(index);

    if (index.column() == ColumnSpeech || index.column() == ColumnShow)
	{
		bool canSpeech = true;

		if (index.column() == ColumnSpeech)
		{
			const qint8 fieldId                           = _vocabulary->fieldId(index.row());
			const VocabularyDatabase::FieldType fieldType = _vocabulary->fieldType(fieldId);
			if (fieldType == VocabularyDatabase::FieldTypeCheckBox)
			{
				canSpeech = false;
			}
		}

		if (canSpeech)
		{
			itemFlags |= Qt::ItemIsUserCheckable;
		}
    }

    return itemFlags;
}

QVariant FieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            switch (section)
			{
                case ColumnTemplateName:
                    return tr("Identifier");
                case ColumnName:
                    return tr("Name");
                case ColumnSpeech:
                    return tr("Speech");
                case ColumnShow:
                    return tr("Show");
                case ColumnLanguage:
                    return tr("Language");
            }
        default:
            return QVariant();
    }
}

QModelIndex FieldsModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}

bool FieldsModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole*/)
{
    if (role != Qt::EditRole && role != Qt::CheckStateRole)
	{
        return false;
    }

    const qint8 fieldId = _vocabulary->fieldId(index.row());

    switch (index.column())
	{
        case ColumnTemplateName:
            _vocabulary->setFieldTemplateName(fieldId, value.toString());
            break;
        case ColumnName:
            _vocabulary->setFieldName(fieldId, value.toString());
            break;
        case ColumnSpeech:
            {
                VocabularyDatabase::FieldAttributes fieldAttributes = _vocabulary->fieldAttributes(fieldId);
                fieldAttributes                                      ^= VocabularyDatabase::FieldAttributeSpeech;
                _vocabulary->setFieldAttributes(fieldId, fieldAttributes);
            }
            break;
        case ColumnShow:
            {
                VocabularyDatabase::FieldAttributes fieldAttributes = _vocabulary->fieldAttributes(fieldId);
                fieldAttributes                                      ^= VocabularyDatabase::FieldAttributeShow;
                _vocabulary->setFieldAttributes(fieldId, fieldAttributes);
            }
            break;
        case ColumnLanguage:
            _vocabulary->setFieldLanguage(fieldId, static_cast<VocabularyDatabase::FieldLanguage>(value.toUInt()));
    }

    emit dataChanged(index, index);
    return true;
}