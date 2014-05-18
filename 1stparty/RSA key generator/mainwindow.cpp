#include "mainwindow.h"

#include <QtWidgets/QFileDialog>
#include "../../3rdparty/Crypto++/source/rsa.h"
#include "../../3rdparty/Crypto++/source/osrng.h"
#include "../../3rdparty/Crypto++/source/files.h"

MainWindow::MainWindow(QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QMainWindow(parent, flags)
{
	_ui.setupUi(this);
} // MainWindow

MainWindow::~MainWindow()
{
} // ~MainWindow

const void MainWindow::on_browsePrivate_clicked(bool checked /* false */)
{
	const QString fileName = QFileDialog::getSaveFileName(this, tr("Private key file"), QString(), tr("private key (*.der)"));
	if (!fileName.isEmpty())
	{
		_ui.privateKey->setText(fileName);
	} // if
} // on_browsePrivate_clicked

const void MainWindow::on_browsePublic_clicked(bool checked /* false */)
{
	const QString fileName = QFileDialog::getSaveFileName(this, tr("Public key file"), QString(), tr("public key (*.der)"));
	if (!fileName.isEmpty())
	{
		_ui.publicKey->setText(fileName);
	} // if
} // on_browsePublic_clicked

const void MainWindow::on_generate_clicked(bool checked /* false */) const
{
	CryptoPP::AutoSeededRandomPool randomPool;
	CryptoPP::InvertibleRSAFunction rsaFunc;
	rsaFunc.GenerateRandomWithKeySize(randomPool, KEY_SIZE);

	const CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(randomPool, KEY_SIZE);
	CryptoPP::FileSink privateFile(_ui.privateKey->text().toLocal8Bit());
	decryptor.DEREncode(privateFile);
	privateFile.MessageEnd();

	const CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(decryptor);
	CryptoPP::FileSink publicFile(_ui.publicKey->text().toLocal8Bit());
	encryptor.DEREncode(publicFile);
	publicFile.MessageEnd();
} // on_generate_clicked