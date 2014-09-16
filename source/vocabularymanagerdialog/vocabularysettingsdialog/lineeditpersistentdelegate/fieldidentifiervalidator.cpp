#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldidentifiervalidator.h"

FieldIdentifierValidator::FieldIdentifierValidator(quint8 row, const FieldsModel *model, QObject *parent /* NULL */) : QValidator(parent), _fieldsModel(model), _row(row)
{
} // FieldIdentifierValidator

FieldIdentifierValidator::~FieldIdentifierValidator()
{
} // ~FieldIdentifierValidator

QValidator::State FieldIdentifierValidator::validate(QString &input, int &pos) const
{
	for (quint8 rowIndex = 0; rowIndex < _fieldsModel->rowCount(); rowIndex++)
	{
		if (rowIndex != _row)
		{
			const QModelIndex modelIndex = _fieldsModel->index(rowIndex, FieldsModel::ColumnTemplateName);
			const QString identifier     = _fieldsModel->data(modelIndex).toString();
			if (input == identifier)
			{
				return Intermediate;
			} // if
		} // if
	} // for

	return Acceptable;
} // validate