#include "vocabularymanagerdialog/vocabularymodel.h"

#ifndef EDITION_FREE
# include "vocabularymanagerdialog/prioritydelegate.h"
#endif

VocabularyModel::VocabularyModel(Vocabulary *vocabulary, quint8 categoryId, QObject *parent /* nullptr */) : QAbstractTableModel(parent), _vocabulary(vocabulary), _categoryId(categoryId)
{
} // VocabularyModel

VocabularyModel::~VocabularyModel()
{
} // ~VocabularyModel

void VocabularyModel::addRow()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _vocabulary->addRecord(_categoryId);
    endInsertRows();
} // addRow

int VocabularyModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return _vocabulary->fieldCount();
} // columnCount

void VocabularyModel::removeRow(quint32 row)
{
    beginRemoveRows(QModelIndex(), row, row);
    _vocabulary->removeRecord(_categoryId, row);
    endRemoveRows();
} // removeRow

int VocabularyModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return _vocabulary->recordCount(_categoryId);
} // rowCount

bool VocabularyModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
	const quint8 fieldId = _vocabulary->fieldId(index.column());
	_vocabulary->setDataText(_categoryId, index.row(), fieldId, value.toString());

	emit dataChanged(index, index);
	return true;
} // setData

QVariant VocabularyModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	const quint8 fieldId = _vocabulary->fieldId(index.column());

	switch (_vocabulary->fieldType(fieldId))
	{
		case VocabularyDatabase::FieldTypeLineEdit:
			switch (role)
			{
				case Qt::DisplayRole:
					return _vocabulary->dataText(_categoryId, index.row(), fieldId);
				default:
					return QVariant();
			} // switch
#ifndef EDITION_FREE
		case VocabularyDatabase::FieldTypeCheckBox:
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
						} // if else
					}
				default:
					return QVariant();
			} // switch
		case VocabularyDatabase::FieldTypeSpinBox:
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
						} // if else
					}
				default:
					return QVariant();
			} // switch
#endif
	} // switch

	return QVariant();
} // data

#ifndef EDITION_FREE
Qt::ItemFlags VocabularyModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags itemFlags = QAbstractItemModel::flags(index);

	const quint8 fieldId = _vocabulary->fieldId(index.column());
	if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
	{
		const VocabularyDatabase::FieldBuiltIn builtIn = _vocabulary->fieldBuiltIn(fieldId);
		switch (builtIn)
		{
			case VocabularyDatabase::FieldBuiltInEnabled:
				itemFlags |= Qt::ItemIsUserCheckable;
				break;
			case VocabularyDatabase::FieldBuiltInPriority:
				itemFlags |= Qt::ItemIsEditable;
		} // switch
	} // if

	return itemFlags;
} // flags
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
            } // if else
        default:
            return QVariant();
    } // switch
} // headerData