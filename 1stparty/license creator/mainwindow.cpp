#include "mainwindow.h"

#include <QtGui/QFileDialog>
#include <QtCore/QUuid>

MainWindow::MainWindow(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QMainWindow(pParent, pFlags)
{
	_umwMainWindow.setupUi(this);

	// preset current date + 1 year
	QDate qdPreset = QDate::currentDate().addYears(1);
	_umwMainWindow.qdeValidTo->setDate(qdPreset);

	on_qpbGenerateUid_clicked();
} // MainWindow

const void MainWindow::on_qpbCreate_clicked(bool checked /* false */)
{
	QString qsFile = QFileDialog::getSaveFileName(this, tr("Create license"), QString(), tr("license (*.lic)"));
	if (qsFile.isEmpty()) {
		return;
	} // if

	QFile qfFile(qsFile);
	qfFile.open(QIODevice::WriteOnly);
	_qxswXmlWriter.setDevice(&qfFile);

	WriteLicense();
} // on_qpbCreate_clicked

const void MainWindow::on_qpbGenerateUid_clicked(bool checked /* false */)
{
	QUuid quUid = QUuid::createUuid();
	_umwMainWindow.qleUid->setText(quUid);
} // on_qpbGenerateUid_clicked

const void MainWindow::WriteLicense()
{
	_qxswXmlWriter.writeStartElement("License");
	WriteLicenseGeneral();
	_qxswXmlWriter.writeStartElement("Personal");
	WriteLicensePersonal();
	_qxswXmlWriter.writeEndElement();
	_qxswXmlWriter.writeEndElement();
} // WriteLicense

const void MainWindow::WriteLicenseGeneral()
{
	_qxswXmlWriter.writeTextElement("UId", _umwMainWindow.qleUid->text());
	_qxswXmlWriter.writeTextElement("ValidTo", _umwMainWindow.qdeValidTo->date().toString(Qt::ISODate));
} // WriteLicenseGeneral

const void MainWindow::WriteLicensePersonal()
{
	_qxswXmlWriter.writeTextElement("FirstName", _umwMainWindow.qleFirstName->text());
	_qxswXmlWriter.writeTextElement("LastName", _umwMainWindow.qleLastName->text());
} // WriteLicensePersonal