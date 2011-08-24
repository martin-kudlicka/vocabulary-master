#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldidentifiervalidator.h"

FieldIdentifierValidator::FieldIdentifierValidator(const int &pRow, const FieldsModel *pModel, QObject *pParent /* NULL */) : QValidator(pParent)
{
	_iRow = pRow;
	_fmModel = pModel;
} // FieldIdentifierValidator

QValidator::State FieldIdentifierValidator::validate(QString &input, int &pos) const
{
	for (int iRow = 0; iRow < _fmModel->rowCount(); iRow++) {
		if (iRow != _iRow) {
			QModelIndex qmiIndex = _fmModel->index(iRow, FieldsModel::ColumnTemplateName);
			QString qsIdentifier = _fmModel->data(qmiIndex).toString();
			if (input == qsIdentifier) {
				return Invalid;
			} // if
		} // if
	} // for

	return Acceptable;
} // validate