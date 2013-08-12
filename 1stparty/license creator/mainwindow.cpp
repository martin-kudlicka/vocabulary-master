#include "mainwindow.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QUuid>
#include <QtCore/QBuffer>
#include "../../3rdparty/Crypto++/source/rsa.h"
#include "../../3rdparty/Crypto++/source/osrng.h"
#include <QtWidgets/QMessageBox>

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

	// buffer
	QBuffer qbLicense;
	qbLicense.open(QIODevice::WriteOnly);
	_qxswXmlWriter.setDevice(&qbLicense);

	// write license
	WriteLicense();
	qbLicense.close();

	// get encrypt key
	QFile qfEncryptKey(":/MainWindow/res/mainwindow/encryptpublic.der");
	qfEncryptKey.open(QIODevice::ReadOnly);
	QByteArray qbaEncryptKey = qfEncryptKey.readAll();

	// encrypt license
    CryptoPP::ArraySource asEncryptKey(reinterpret_cast<const byte *>(qbaEncryptKey.constData()), qbaEncryptKey.size(), true);
    CryptoPP::RSAES_OAEP_SHA_Encryptor roseEncryptor(asEncryptKey);
    CryptoPP::AutoSeededRandomPool asrpRandomPool;
    std::string sEncrypted;
    CryptoPP::ArraySource asEncrypt(reinterpret_cast<const byte *>(qbLicense.data().constData()), qbLicense.size(), true, new CryptoPP::PK_EncryptorFilter(asrpRandomPool, roseEncryptor, new CryptoPP::StringSink(sEncrypted)));
    QByteArray qbaEncrypted = QByteArray(sEncrypted.c_str(), sEncrypted.size());

	// get sign key
	QFile qfSignKey(":/MainWindow/res/mainwindow/signprivate.der");
	qfSignKey.open(QIODevice::ReadOnly);
	QByteArray qbaSignKey = qfSignKey.readAll();

	// sign encrypted license
    CryptoPP::ArraySource asSignKey(reinterpret_cast<const byte *>(qbaSignKey.constData()), qbaSignKey.size(), true);
    CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::Signer sSigner(asSignKey);
    std::string sSigned;
    CryptoPP::ArraySource asSign(reinterpret_cast<const byte *>(qbaEncrypted.constData()), qbaEncrypted.size(), true, new CryptoPP::SignerFilter(asrpRandomPool, sSigner, new CryptoPP::StringSink(sSigned)));
    QByteArray qbaSigned = QByteArray(sSigned.c_str(), sSigned.size());

	// get size of encrypted data
	qint16 qi16Size = qbaEncrypted.size();
	QByteArray qbaEncryptedSize(reinterpret_cast<const char *>(&qi16Size), sizeof(qi16Size));

	// write license to file
	QFile qfFile(qsFile);
	qfFile.open(QIODevice::WriteOnly);
	qfFile.write(qbaEncryptedSize + qbaEncrypted + qbaSigned);

	QMessageBox::information(this, windowTitle(), tr("License created, encrypted and signed."));
} // on_qpbCreate_clicked

const void MainWindow::on_qpbGenerateUid_clicked(bool checked /* false */)
{
	QUuid quUid = QUuid::createUuid();
	_umwMainWindow.qleUid->setText(quUid.toString());
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
	_qxswXmlWriter.writeTextElement("Email", _umwMainWindow.qleEmail->text());
} // WriteLicensePersonal