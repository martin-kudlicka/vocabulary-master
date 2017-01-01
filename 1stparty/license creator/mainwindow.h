#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QtCore/QXmlStreamWriter>

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
             MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
    virtual ~MainWindow() Q_DECL_OVERRIDE;

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