#ifndef FIELDIDENTIFIERVALIDATOR_H
#define FIELDIDENTIFIERVALIDATOR_H

#include <QtGui/QValidator>
#include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"

class FieldIdentifierValidator : public QValidator
{
	public:
		FieldIdentifierValidator(quint8 row, const FieldsModel *model, QObject *parent = NULL);

	private:
		const FieldsModel *_fieldsModel;
		      quint8       _row;

		virtual ~FieldIdentifierValidator() override;

		virtual State validate(QString &input, int &pos) const override;
}; // FieldIdentifierValidator

#endif // FIELDIDENTIFIERVALIDATOR_H