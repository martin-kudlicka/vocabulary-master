#ifndef MARKLINEEDIT_H
#define MARKLINEEDIT_H

#include <QtWidgets/QLineEdit>

class MarkLineEdit : public QLineEdit
{
  Q_OBJECT

  public:
    MarkLineEdit(QWidget *parent = nullptr);
    MarkLineEdit(const QString &contents, QWidget *parent = nullptr);

  private:
    bool _focusedIn;

    virtual ~MarkLineEdit() override;

    void initialize();

    virtual void focusInEvent   (QFocusEvent *e)    override;
    virtual void mousePressEvent(QMouseEvent *e)    override;
    virtual void showEvent      (QShowEvent *event) override;
};

#endif