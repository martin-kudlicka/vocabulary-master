#ifndef FIELDIDENTIFIERVALIDATOR_H
#define FIELDIDENTIFIERVALIDATOR_H

class FieldsModel;

class FieldIdentifierValidator : public QValidator
{
  public:
    FieldIdentifierValidator(quintptr row, const FieldsModel *model, QObject *parent);

  private:
    const FieldsModel *_fieldsModel;
          quintptr     _row;

    virtual ~FieldIdentifierValidator() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual State validate(QString &input, int &pos) const Q_DECL_OVERRIDE;
};

#endif