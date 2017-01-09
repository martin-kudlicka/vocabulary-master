#include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"

#include "vocabulary.h"

FieldsModel::FieldsModel(Vocabulary *vocabulary, QObject *parent /* Q_NULLPTR */) : QAbstractItemModel(parent), _vocabulary(vocabulary)
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

void FieldsModel::removeRow(quintptr row)
{
	beginRemoveRows(QModelIndex(), row, row);
	const qint8 fieldId = _vocabulary->fieldId(row);
	_vocabulary->removeField(fieldId);
	endRemoveRows();
}

void FieldsModel::swap(quintptr sourceRow, quintptr destinationRow)
{
	const qint8 sourceFieldId      = _vocabulary->fieldId(sourceRow);
	const qint8 destinationFieldId = _vocabulary->fieldId(destinationRow);
	_vocabulary->swapFields(sourceFieldId, destinationFieldId);

	emit dataChanged(index(sourceRow, 0), index(sourceRow, static_cast<int>(Column::Count) - 1));
	emit dataChanged(index(destinationRow, 0), index(destinationRow, static_cast<int>(Column::Count) - 1));
}

QVariant FieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	const qint8 fieldId = _vocabulary->fieldId(index.row());

	switch (index.column())
	{
		case Column::TemplateName:
			switch (role)
			{
				case Qt::DisplayRole:
					return _vocabulary->fieldTemplateName(fieldId);
				default:
					return QVariant();
			}
		case Column::Name:
			switch (role)
			{
				case Qt::DisplayRole:
					return _vocabulary->fieldName(fieldId);
				default:
					return QVariant();
			}
        case Column::Speech:
            switch (role)
			{
				case Qt::CheckStateRole:
                    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::Speech))
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
        case Column::Show:
            switch (role)
			{
                case Qt::CheckStateRole:
                    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::Show))
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
		case Column::Language:
			switch (role)
			{
				case Qt::EditRole:
					return static_cast<quintptr>(_vocabulary->fieldLanguage(fieldId));
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

int FieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
	return static_cast<int>(Column::Count);
}

Qt::ItemFlags FieldsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags itemFlags = QAbstractItemModel::flags(index);

    if (index.column() == static_cast<int>(Column::Speech) || index.column() == static_cast<int>(Column::Show))
	{
		bool canSpeech = true;

    if (index.column() == static_cast<int>(Column::Speech))
		{
			const qint8 fieldId                           = _vocabulary->fieldId(index.row());
			const VocabularyDatabase::FieldType fieldType = _vocabulary->fieldType(fieldId);
			if (fieldType == VocabularyDatabase::FieldType::CheckBox)
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
                case Column::TemplateName:
                    return tr("Identifier");
                case Column::Name:
                    return tr("Name");
                case Column::Speech:
                    return tr("Speech");
                case Column::Show:
                    return tr("Show");
                case Column::Language:
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
        case Column::TemplateName:
            _vocabulary->setFieldTemplateName(fieldId, value.toString());
            break;
        case Column::Name:
            _vocabulary->setFieldName(fieldId, value.toString());
            break;
        case Column::Speech:
            {
                VocabularyDatabase::FieldAttributes fieldAttributes = _vocabulary->fieldAttributes(fieldId);
                fieldAttributes                                      ^= VocabularyDatabase::FieldAttribute::Speech;
                _vocabulary->setFieldAttributes(fieldId, fieldAttributes);
            }
            break;
        case Column::Show:
            {
                VocabularyDatabase::FieldAttributes fieldAttributes = _vocabulary->fieldAttributes(fieldId);
                fieldAttributes                                      ^= VocabularyDatabase::FieldAttribute::Show;
                _vocabulary->setFieldAttributes(fieldId, fieldAttributes);
            }
            break;
        case Column::Language:
            _vocabulary->setFieldLanguage(fieldId, static_cast<VocabularyDatabase::FieldLanguage>(value.toUInt()));
    }

    emit dataChanged(index, index);
    return true;
}