#include "mainwindow.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QUuid>
#include <QtCore/QBuffer>
#include "../../3rdparty/Crypto++/source/rsa.h"
#include "../../3rdparty/Crypto++/source/osrng.h"
#include <QtWidgets/QMessageBox>

MainWindow::MainWindow(QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QMainWindow(parent, flags)
{
	_ui.setupUi(this);

	// preset current date + 1 year
	QDate preset = QDate::currentDate().addYears(1);
	_ui.validTo->setDate(preset);

	on_generateUid_clicked();
} // MainWindow

const void MainWindow::writeLicense()
{
	_xmlStreamWriter.writeStartElement("License");
	writeLicenseGeneral();
	_xmlStreamWriter.writeStartElement("Personal");
	writeLicensePersonal();
	_xmlStreamWriter.writeEndElement();
	_xmlStreamWriter.writeEndElement();
} // writeLicense

const void MainWindow::writeLicenseGeneral()
{
	_xmlStreamWriter.writeTextElement("UId", _ui.uid->text());
	_xmlStreamWriter.writeTextElement("ValidTo", _ui.validTo->date().toString(Qt::ISODate));
} // writeLicenseGeneral

const void MainWindow::writeLicensePersonal()
{
	_xmlStreamWriter.writeTextElement("FirstName", _ui.firstName->text());
	_xmlStreamWriter.writeTextElement("LastName", _ui.lastName->text());
	_xmlStreamWriter.writeTextElement("Email", _ui.email->text());
} // writeLicensePersonal

const void MainWindow::on_create_clicked(bool checked /* false */)
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Create license"), QString(), tr("license (*.lic)"));
	if (fileName.isEmpty())
	{
		return;
	} // if

	// buffer
	QBuffer license;
	license.open(QIODevice::WriteOnly);
	_xmlStreamWriter.setDevice(&license);

	// write license
	writeLicense();
	license.close();

	// get encrypt key
	QFile encryptKeyFile(":/MainWindow/res/mainwindow/encryptpublic.der");
	encryptKeyFile.open(QIODevice::ReadOnly);
	QByteArray encryptKey = encryptKeyFile.readAll();

	// encrypt license
	CryptoPP::ArraySource encryptKeyBuffer(reinterpret_cast<const byte *>(encryptKey.constData()), encryptKey.size(), true);
	CryptoPP::RSAES_OAEP_SHA_Encryptor roseEncryptor(encryptKeyBuffer);
	CryptoPP::AutoSeededRandomPool randomPool;
	std::string encryptedString;
	CryptoPP::ArraySource encryptedBuffer(reinterpret_cast<const byte *>(license.data().constData()), license.size(), true, new CryptoPP::PK_EncryptorFilter(randomPool, roseEncryptor, new CryptoPP::StringSink(encryptedString)));
	QByteArray encryptedData = QByteArray(encryptedString.c_str(), encryptedString.size());

	// get sign key
	QFile signKeyFile(":/MainWindow/res/mainwindow/signprivate.der");
	signKeyFile.open(QIODevice::ReadOnly);
	QByteArray signKey = signKeyFile.readAll();

	// sign encrypted license
	CryptoPP::ArraySource signKeyBuffer(reinterpret_cast<const byte *>(signKey.constData()), signKey.size(), true);
	CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::Signer signer(signKeyBuffer);
	std::string signedString;
	CryptoPP::ArraySource signedBuffer(reinterpret_cast<const byte *>(encryptedData.constData()), encryptedData.size(), true, new CryptoPP::SignerFilter(randomPool, signer, new CryptoPP::StringSink(signedString)));
	QByteArray signedData = QByteArray(signedString.c_str(), signedString.size());

	// get size of encrypted data
	qint16 encryptedSize = encryptedData.size();
	QByteArray encryptedSizeData(reinterpret_cast<const char *>(&encryptedSize), sizeof(encryptedSize));

	// write license to file
	QFile licenseFile(fileName);
	licenseFile.open(QIODevice::WriteOnly);
	licenseFile.write(encryptedSizeData + encryptedData + signedData);

	QMessageBox::information(this, windowTitle(), tr("License created, encrypted and signed."));
} // on_create_clicked

const void MainWindow::on_generateUid_clicked(bool checked /* false */)
{
	QUuid uuid = QUuid::createUuid();
	_ui.uid->setText(uuid.toString());
} // on_generateUid_clicked