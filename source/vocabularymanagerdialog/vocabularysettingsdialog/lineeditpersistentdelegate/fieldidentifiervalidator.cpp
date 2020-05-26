#include "pch.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldidentifiervalidator.h"

#include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"

FieldIdentifierValidator::FieldIdentifierValidator(quintptr row, const FieldsModel *model, QObject *parent) : QValidator(parent), _fieldsModel(model), _row(row)
{
}

QValidator::State FieldIdentifierValidator::validate(QString &input, int &pos) const
{
  Q_UNUSED(pos);

  for (auto rowIndex = 0; rowIndex < _fieldsModel->rowCount(); ++rowIndex)
  {
    if (static_cast<decltype(_row)>(rowIndex) != _row)
    {
      const auto modelIndex = _fieldsModel->index(rowIndex, static_cast<int>(FieldsModel::Column::TemplateName));
      const auto identifier = _fieldsModel->data(modelIndex).toString();
      if (input == identifier)
      {
        return Intermediate;
      }
    }
  }

  return Acceptable;
}