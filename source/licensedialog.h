#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QtWidgets/QDialog>
#include <ui_licensedialog.h>

class License;
class Settings;

class LicenseDialog : public QDialog
{
  Q_OBJECT

  public:
             LicenseDialog(License *license, Settings *settings, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~LicenseDialog() Q_DECL_OVERRIDE;

  private:
    bool              _refreshed;
    License          *_license;
    Settings         *_settings;
    Ui::LicenseDialog _ui;

    void fillLicenseInfo() const;

    private Q_SLOTS:
    void on_eula_clicked(bool checked = false);
    void on_load_clicked(bool checked = false);
    void on_ok_clicked  (bool checked = false);
};

#endif