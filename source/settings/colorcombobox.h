#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QtWidgets/QComboBox>

class ColorComboBox : public QComboBox
{
  public:
    ColorComboBox(QWidget *parent = nullptr);

  private:
    virtual ~ColorComboBox() override;

    virtual void paintEvent(QPaintEvent * e) override;
};

#endif