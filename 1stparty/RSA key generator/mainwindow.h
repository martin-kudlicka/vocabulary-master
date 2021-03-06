#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
             MainWindow();
    virtual ~MainWindow() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    Ui::MainWindow _ui;

  private Q_SLOTS:
    void on_browsePrivate_clicked(bool checked = false);
    void on_browsePublic_clicked (bool checked = false);
    void on_generate_clicked     (bool checked = false) const;
};

#endif