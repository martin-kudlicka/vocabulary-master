#include "vocabularyorganizerdialog/vocabularyorganizermodel.h"

#include "vocabularyorganizer.h"
#include <QtCore/QDir>

VocabularyOrganizerModel::VocabularyOrganizerModel(VocabularyOrganizer *organizer, QWidget *parent) : _organizer(organizer), _parent(parent)
{
}

void VocabularyOrganizerModel::addRow()
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  endInsertRows();
}

void VocabularyOrganizerModel::removeRow(quintptr row)
{
  beginRemoveRows(QModelIndex(), row, row);
  endRemoveRows();
}

int VocabularyOrganizerModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return static_cast<int>(Column::Count);
}

QVariant VocabularyOrganizerModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  switch (index.column())
  {
    case Column::VocabularyFile:
      switch (role)
      {
        case Qt::DisplayRole:
        {
          const auto file = _organizer->vocabularyInfo(index.row()).vocabularyInfo.filePath;
          return QDir::toNativeSeparators(file);
        }
        default:
          return QVariant();
      }
    case Column::Enabled:
      switch (role)
      {
        case Qt::CheckStateRole:
        {
          const auto enabled = _organizer->vocabularyInfo(index.row()).vocabularyInfo.enabled;
          if (enabled)
          {
            return Qt::Checked;
          }
          else
          {
            return Qt::Unchecked;
          }
        }
        default:
          return QVariant();
      }
    default:
      return QVariant();
  }
}

Qt::ItemFlags VocabularyOrganizerModel::flags(const QModelIndex &index) const
{
  auto itemFlags = QAbstractItemModel::flags(index);

  if (index.column() == static_cast<int>(Column::Enabled))
  {
    itemFlags |= Qt::ItemIsUserCheckable;
  }

  return itemFlags;
}

QVariant VocabularyOrganizerModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
  switch (role)
  {
    case Qt::DisplayRole:
      switch (section)
      {
        case Column::VocabularyFile:
          return tr("Vocabulary");
        case Column::Enabled:
          return tr("Enabled");
      }
    default:
      return QVariant();
  }
}

int VocabularyOrganizerModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  if (parent.isValid())
  {
    return 0;
  }

  return _organizer->vocabularyCount();
}

bool VocabularyOrganizerModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
  switch (index.column())
  {
    case Column::Enabled:
      _organizer->setVocabularyEnabled(index.row(), value.toBool(), _parent);

      emit dataChanged(index, index);
      return true;
    default:
      return false;
  }
}