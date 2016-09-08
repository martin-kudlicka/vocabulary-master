#ifndef FIELDLINEEDIT_H
#define FIELDLINEEDIT_H

#include <QtWidgets/QLineEdit>

class FieldLineEdit : public QLineEdit
{
  Q_OBJECT

  public:
    FieldLineEdit(QWidget *parent = nullptr);

  private:
    QString _defaultSheet;

    virtual ~FieldLineEdit() override;

    virtual void showEvent(QShowEvent *event) override;

  private slots:
    void on_textEdited(const QString &text);
};

#endif