#ifndef MARKLINEEDIT_H
#define MARKLINEEDIT_H

class MarkLineEdit : public QLineEdit
{
  Q_OBJECT

  public:
    MarkLineEdit(QWidget *parent);
    MarkLineEdit(const QString &contents, QWidget *parent);

  private:
    bool _focusedIn;

    virtual ~MarkLineEdit() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    void initialize();

    virtual void focusInEvent   (QFocusEvent *e)    Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *e)    Q_DECL_OVERRIDE;
    virtual void showEvent      (QShowEvent *event) Q_DECL_OVERRIDE;
};

#endif