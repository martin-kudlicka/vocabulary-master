#ifndef FIELDLINEEDIT_H
#define FIELDLINEEDIT_H

#include <QtWidgets/QLineEdit>

class FieldLineEdit : public QLineEdit
{
  Q_OBJECT

  public:
    FieldLineEdit(QWidget *parent = Q_NULLPTR);

  private:
    QString _defaultSheet;

    virtual ~FieldLineEdit() Q_DECL_OVERRIDE;

    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

  private Q_SLOTS:
    void on_textEdited(const QString &text);
};

#endif