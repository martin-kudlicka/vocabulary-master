#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldidentifiervalidator.h"

#include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"

FieldIdentifierValidator::FieldIdentifierValidator(quintptr row, const FieldsModel *model, QObject *parent /* Q_NULLPTR */) : QValidator(parent), _fieldsModel(model), _row(row)
{
}

FieldIdentifierValidator::~FieldIdentifierValidator()
{
}

QValidator::State FieldIdentifierValidator::validate(QString &input, int &pos) const
{
  for (auto rowIndex = 0; rowIndex < _fieldsModel->rowCount(); rowIndex++)
  {
    if (rowIndex != _row)
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