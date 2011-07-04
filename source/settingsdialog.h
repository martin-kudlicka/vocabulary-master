#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui/QDialog>
#include "ui_settingsdialog.h"

#include "settings.h"

class SettingsDialog : public QDialog
{
	Q_OBJECT

	public:
		SettingsDialog(Settings *pSettings, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		Settings *_sSettings;
		Ui::qdUiSettings _usdSettingsDialog;

        void accept();
        const void FillOptions();
        const void SaveOptions();

	private slots:
		void on_qsbWordsFrequency_valueChanged(int i);
}; // MainWindow

#endif // SETTINGSDIALOG_H