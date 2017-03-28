#include "mainwindow.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QUuid>
#include <QtCore/QBuffer>
#include "../../3rdparty/Crypto++/source/rsa.h"
#include "../../3rdparty/Crypto++/source/osrng.h"
#include <QtWidgets/QMessageBox>

MainWindow::MainWindow(QWidget *parent /* Q_NULLPTR */, Qt::WindowFlags flags /* 0 */) : QMainWindow(parent, flags)
{
  _ui.setupUi(this);

  // preset current date + 1 year
  const auto presetDate = QDate::currentDate().addYears(1);
  _ui.validTo->setDate(presetDate);

  _ui.generateUid->click();
}

MainWindow::~MainWindow()
{
}

void MainWindow::writeLicense()
{
  _xmlStreamWriter.writeStartElement("License");
  writeLicenseGeneral();
  _xmlStreamWriter.writeStartElement("Personal");
  writeLicensePersonal();
  _xmlStreamWriter.writeEndElement();
  _xmlStreamWriter.writeEndElement();
}

void MainWindow::writeLicenseGeneral()
{
  _xmlStreamWriter.writeTextElement("UId", _ui.uid->text());
  _xmlStreamWriter.writeTextElement("ValidTo", _ui.validTo->date().toString(Qt::ISODate));
}

void MainWindow::writeLicensePersonal()
{
  _xmlStreamWriter.writeTextElement("FirstName", _ui.firstName->text());
  _xmlStreamWriter.writeTextElement("LastName", _ui.lastName->text());
  _xmlStreamWriter.writeTextElement("Email", _ui.email->text());
}

void MainWindow::on_create_clicked(bool checked /* false */)
{
  const auto fileName = QFileDialog::getSaveFileName(this, tr("Create license"), QString(), tr("license (*.lic)"));
  if (fileName.isEmpty())
  {
    return;
  }

  // buffer
  QBuffer licenseBuffer;
  licenseBuffer.open(QIODevice::WriteOnly);
  _xmlStreamWriter.setDevice(&licenseBuffer);

  // write license
  writeLicense();
  licenseBuffer.close();

  // get encrypt key
  QFile encryptKeyFile(":/MainWindow/res/mainwindow/encryptpublic.der");
  encryptKeyFile.open(QIODevice::ReadOnly);
  const auto encryptKeyData = encryptKeyFile.readAll();

  // encrypt license
  CryptoPP::ArraySource encryptKeyBuffer(reinterpret_cast<const byte *>(encryptKeyData.constData()), encryptKeyData.size(), true);
  const CryptoPP::RSAES_OAEP_SHA_Encryptor roseEncryptor(encryptKeyBuffer);
  CryptoPP::AutoSeededRandomPool randomPool;
  std::string encryptedString;
  const CryptoPP::ArraySource encryptedBuffer(reinterpret_cast<const byte *>(licenseBuffer.data().constData()), licenseBuffer.size(), true, new CryptoPP::PK_EncryptorFilter(randomPool, roseEncryptor, new CryptoPP::StringSink(encryptedString)));
  const QByteArray encryptedData(encryptedString.c_str(), encryptedString.size());

  // get sign key
  QFile signKeyFile(":/MainWindow/res/mainwindow/signprivate.der");
  signKeyFile.open(QIODevice::ReadOnly);
  const auto signKeyData = signKeyFile.readAll();

  // sign encrypted license
  CryptoPP::ArraySource signKeyBuffer(reinterpret_cast<const byte *>(signKeyData.constData()), signKeyData.size(), true);
  const CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::Signer signer(signKeyBuffer);
  std::string signedString;
  const CryptoPP::ArraySource signedBuffer(reinterpret_cast<const byte *>(encryptedData.constData()), encryptedData.size(), true, new CryptoPP::SignerFilter(randomPool, signer, new CryptoPP::StringSink(signedString)));
  const QByteArray signedData(signedString.c_str(), signedString.size());

  // get size of encrypted data
  const auto encryptedSize = encryptedData.size();
  const QByteArray encryptedSizeData(reinterpret_cast<const char *>(&encryptedSize), sizeof(encryptedSize));

  // write license to file
  QFile licenseFile(fileName);
  licenseFile.open(QIODevice::WriteOnly);
  licenseFile.write(encryptedSizeData + encryptedData + signedData);

  QMessageBox::information(this, windowTitle(), tr("License created, encrypted and signed."));
}

void MainWindow::on_generateUid_clicked(bool checked /* false */)
{
  const auto uuid = QUuid::createUuid();
  _ui.uid->setText(uuid.toString());
}