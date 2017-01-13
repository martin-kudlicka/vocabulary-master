#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QtWidgets/QComboBox>

class ColorComboBox : public QComboBox
{
  public:
    ColorComboBox(QWidget *parent = Q_NULLPTR);

  private:
    virtual ~ColorComboBox() Q_DECL_OVERRIDE;

    virtual void paintEvent(QPaintEvent * e) Q_DECL_OVERRIDE;
};

#endif