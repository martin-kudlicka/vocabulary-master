#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QtCore/QXmlStreamWriter>

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
             MainWindow();
    virtual ~MainWindow() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    QXmlStreamWriter _xmlStreamWriter;
    Ui::MainWindow   _ui;

    void writeLicense        ();
    void writeLicenseGeneral ();
    void writeLicensePersonal();

  private Q_SLOTS:
    void on_create_clicked     (bool checked = false);
    void on_generateUid_clicked(bool checked = false);
};

#endif