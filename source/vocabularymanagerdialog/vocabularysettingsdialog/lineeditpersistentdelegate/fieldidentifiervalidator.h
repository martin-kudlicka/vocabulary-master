#ifndef FIELDIDENTIFIERVALIDATOR_H
#define FIELDIDENTIFIERVALIDATOR_H

#include <QtGui/QValidator>

class FieldsModel;

class FieldIdentifierValidator : public QValidator
{
  public:
    FieldIdentifierValidator(quintptr row, const FieldsModel *model, QObject *parent = Q_NULLPTR);

  private:
    const FieldsModel *_fieldsModel;
          quintptr     _row;

    virtual ~FieldIdentifierValidator() Q_DECL_OVERRIDE;

    virtual State validate(QString &input, int &pos) const Q_DECL_OVERRIDE;
};

#endif