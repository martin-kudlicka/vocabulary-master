#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QtWidgets/QComboBox>

class ColorComboBox : public QComboBox
{
  public:
    ColorComboBox(QWidget *parent);

  private:
    virtual ~ColorComboBox() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual void paintEvent(QPaintEvent * e) Q_DECL_OVERRIDE;
};

#endif