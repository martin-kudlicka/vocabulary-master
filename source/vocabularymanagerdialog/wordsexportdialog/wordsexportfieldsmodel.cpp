#include "vocabularymanagerdialog/wordsexportdialog/wordsexportfieldsmodel.h"

#include "vocabulary.h"

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
    auto count = 0;
    for (auto vocabularyFieldId : _vocabulary->fieldIds())
    {
      if (_vocabulary->fieldBuiltIn(vocabularyFieldId) == VocabularyDatabase::FieldBuiltIn::None)
      {
        count++;
      }
    }
    return count;
  }

  return 0;
}

int WordsExportFieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return static_cast<int>(Column::Count);
}

QVariant WordsExportFieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  switch (role)
  {
    case Qt::DisplayRole:
      switch (index.column())
      {
        case static_cast<int>(Column::Language):
        {
          const auto languageFieldId = fieldId(index.row());
          const auto language        = _vocabulary->fieldLanguage(languageFieldId);
          return _vocabulary->languageName(language);
        }
        case static_cast<int>(Column::Name):
        {
          const auto languageFieldId = fieldId(index.row());
          return _vocabulary->fieldName(languageFieldId);
        }
        case static_cast<int>(Column::Mark):
          const auto languageFieldId = fieldId(index.row());
          const auto templateText    = _vocabulary->fieldTemplateName(languageFieldId);
          return TEMPLATE_MARK.arg(templateText);
      }
    default:
      return QVariant();
  }
}

quintptr WordsExportFieldsModel::fieldId(quintptr row) const
{
  auto num = 0;
  for (auto vocabularyFieldId : _vocabulary->fieldIds())
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
        case static_cast<int>(Column::Language):
          return tr("Language");
        case static_cast<int>(Column::Name):
          return tr("Name");
        case static_cast<int>(Column::Mark):
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