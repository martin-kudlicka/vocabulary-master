#ifndef MARKLINEEDIT_H
#define MARKLINEEDIT_H

#include <QtWidgets/QLineEdit>

class MarkLineEdit : public QLineEdit
{
  Q_OBJECT

  public:
    MarkLineEdit(QWidget *parent = Q_NULLPTR);
    MarkLineEdit(const QString &contents, QWidget *parent = Q_NULLPTR);

  private:
    bool _focusedIn;

    virtual ~MarkLineEdit() Q_DECL_OVERRIDE;

    void initialize();

    virtual void focusInEvent   (QFocusEvent *e)    Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *e)    Q_DECL_OVERRIDE;
    virtual void showEvent      (QShowEvent *event) Q_DECL_OVERRIDE;
};

#endif