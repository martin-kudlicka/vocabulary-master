#include "pch.h"
#include "vocabularymanagerdialog/wordsimportdialog/wordsimportfieldsmodel.h"

#include "vocabulary.h"
#include "vocabularymanagerdialog/prioritydelegate.h"

WordsImportFieldsModel::WordsImportFieldsModel(const QSharedPointer<Vocabulary> &vocabulary) : _vocabulary(vocabulary)
{
  for (decltype(_vocabulary->fieldCount()) fieldIndex = 0; fieldIndex < _vocabulary->fieldCount(); ++fieldIndex)
  {
    _editorData.append(QString());
  }
}

QVariant WordsImportFieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  switch (index.column())
  {
    case Column::Language:
      switch (role)
      {
        case Qt::DisplayRole:
        {
          const auto fieldId  = _vocabulary->fieldId(index.row());
          const auto language = _vocabulary->fieldLanguage(fieldId);
          return _vocabulary->languageName(language);
        }
        default:
          return QVariant();
      }
    case Column::Name:
      switch (role)
      {
        case Qt::DisplayRole:
        {
          const auto fieldId = _vocabulary->fieldId(index.row());
          return _vocabulary->fieldName(fieldId);
        }
        default:
          return QVariant();
      }
    case Column::Editor:
    {
      const auto fieldId = _vocabulary->fieldId(index.row());

      if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
      {
        const auto builtIn = _vocabulary->fieldBuiltIn(fieldId);
        switch (builtIn)
        {
          case VocabularyDatabase::FieldBuiltIn::Enabled:
            switch (role)
            {
              case Qt::EditRole:
                Q_FALLTHROUGH();
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
          case VocabularyDatabase::FieldBuiltIn::Priority:
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
  Q_UNUSED(parent);

  return createIndex(row, column);
}

int WordsImportFieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  if (parent.isValid())
  {
    return 0;
  }

  return _vocabulary->fieldCount();
}

int WordsImportFieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  Q_UNUSED(parent);

  return static_cast<int>(Column::Count);
}

Qt::ItemFlags WordsImportFieldsModel::flags(const QModelIndex &index) const
{
  auto itemFlags = QAbstractItemModel::flags(index);

  if (index.column() == static_cast<int>(Column::Editor))
  {
    const auto fieldId   = _vocabulary->fieldId(index.row());
    const auto fieldType = _vocabulary->fieldType(fieldId);
    if (fieldType == VocabularyDatabase::FieldType::CheckBox)
    {
      itemFlags |= Qt::ItemIsUserCheckable;
    }
  }

  return itemFlags;
}

QVariant WordsImportFieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
  Q_UNUSED(orientation);

  switch (role)
  {
    case Qt::DisplayRole:
      switch (section)
      {
        case Column::Language:
          return tr("Language");
        case Column::Name:
          return tr("Name");
        case Column::Editor:
          return tr("Pattern");
      }
    default:
      return QVariant();
  }
}

QModelIndex WordsImportFieldsModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);

  return QModelIndex();
}

bool WordsImportFieldsModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
  Q_UNUSED(role);

  _editorData[index.row()] = value.toString();

  emit dataChanged(index, index);
  return true;
}