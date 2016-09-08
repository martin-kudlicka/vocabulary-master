#ifndef FIELDIDENTIFIERVALIDATOR_H
#define FIELDIDENTIFIERVALIDATOR_H

#include <QtGui/QValidator>

class FieldsModel;

class FieldIdentifierValidator : public QValidator
{
  public:
    FieldIdentifierValidator(quintptr row, const FieldsModel *model, QObject *parent = nullptr);

  private:
    const FieldsModel *_fieldsModel;
          quintptr     _row;

    virtual ~FieldIdentifierValidator() override;

    virtual State validate(QString &input, int &pos) const override;
};

#endif