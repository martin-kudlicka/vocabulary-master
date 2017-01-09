#include "vocabularymanagerdialog/vocabularymodel.h"

#ifndef EDITION_FREE
# include "vocabularymanagerdialog/prioritydelegate.h"
#endif

VocabularyModel::VocabularyModel(Vocabulary *vocabulary, quint8 categoryId, QObject *parent /* Q_NULLPTR */) : QAbstractTableModel(parent), _vocabulary(vocabulary), _categoryId(categoryId)
{
}

VocabularyModel::~VocabularyModel()
{
}

void VocabularyModel::addRow()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _vocabulary->addRecord(_categoryId);
    endInsertRows();
}

int VocabularyModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return _vocabulary->fieldCount();
}

void VocabularyModel::removeRow(quint32 row)
{
    beginRemoveRows(QModelIndex(), row, row);
    _vocabulary->removeRecord(_categoryId, row);
    endRemoveRows();
}

int VocabularyModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return _vocabulary->recordCount(_categoryId);
}

bool VocabularyModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
	const quint8 fieldId = _vocabulary->fieldId(index.column());
	_vocabulary->setDataText(_categoryId, index.row(), fieldId, value.toString());

	emit dataChanged(index, index);
	return true;
}

QVariant VocabularyModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	const quint8 fieldId = _vocabulary->fieldId(index.column());

	switch (_vocabulary->fieldType(fieldId))
	{
		case VocabularyDatabase::FieldType::LineEdit:
			switch (role)
			{
				case Qt::DisplayRole:
					return _vocabulary->dataText(_categoryId, index.row(), fieldId);
				default:
					return QVariant();
			}
#ifndef EDITION_FREE
		case VocabularyDatabase::FieldType::CheckBox:
			switch (role)
			{
				case Qt::CheckStateRole:
					{
						const QString checked = _vocabulary->dataText(_categoryId, index.row(), fieldId);
						if (checked.isEmpty())
						{
							return Qt::Checked;
						}
						else
						{
							return checked.toUInt();
						}
					}
				default:
					return QVariant();
			}
		case VocabularyDatabase::FieldType::SpinBox:
			switch (role)
			{
				case Qt::DisplayRole:
					{
						const QString priority = _vocabulary->dataText(_categoryId, index.row(), fieldId);
						if (priority.isEmpty())
						{
							return PriorityDelegate::RECORD_PRIORITY_MIN;
						}
						else
						{
							return priority.toUInt();
						}
					}
				default:
					return QVariant();
			}
#endif
	}

	return QVariant();
}

#ifndef EDITION_FREE
Qt::ItemFlags VocabularyModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags itemFlags = QAbstractItemModel::flags(index);

	const quint8 fieldId = _vocabulary->fieldId(index.column());
	if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
	{
		const VocabularyDatabase::FieldBuiltIn builtIn = _vocabulary->fieldBuiltIn(fieldId);
		switch (builtIn)
		{
			case VocabularyDatabase::FieldBuiltIn::Enabled:
				itemFlags |= Qt::ItemIsUserCheckable;
				break;
			case VocabularyDatabase::FieldBuiltIn::Priority:
				itemFlags |= Qt::ItemIsEditable;
		}
	}

	return itemFlags;
}
#endif

QVariant VocabularyModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            if (orientation == Qt::Horizontal)
			{
                return _vocabulary->fieldName(_vocabulary->fieldId(section));
            }
			else
			{
                return section + 1;
            }
        default:
            return QVariant();
    }
}