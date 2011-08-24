#ifndef FIELDIDENTIFIERVALIDATOR_H
#define FIELDIDENTIFIERVALIDATOR_H

#include <QtGui/QValidator>
#include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"

class FieldIdentifierValidator : public QValidator
{
	public:
		FieldIdentifierValidator(const int &pRow, const FieldsModel *pModel, QObject *pParent = NULL);

	private:
		const FieldsModel *_fmModel;
		int _iRow;

		virtual State validate(QString &input, int &pos) const;
}; // FieldIdentifierValidator

#endif // FIELDIDENTIFIERVALIDATOR_H