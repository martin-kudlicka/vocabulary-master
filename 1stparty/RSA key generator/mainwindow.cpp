#include "mainwindow.h"

#include <QtWidgets/QFileDialog>
#include "../../3rdparty/Crypto++/source/rsa.h"
#include "../../3rdparty/Crypto++/source/osrng.h"
#include "../../3rdparty/Crypto++/source/files.h"

MainWindow::MainWindow(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QMainWindow(pParent, pFlags)
{
	_qmwMainWindow.setupUi(this);
} // MainWindow

const void MainWindow::on_qpbBrowsePublic_clicked(bool checked /* false */)
{
	QString qsFile = QFileDialog::getSaveFileName(this, tr("Public key file"), QString(), tr("public key (*.der)"));
	if (!qsFile.isEmpty()) {
		_qmwMainWindow.qlePublic->setText(qsFile);
	} // if
} // on_qpbBrowsePublic_clicked

const void MainWindow::on_qpbBrowsePrivate_clicked(bool checked /* false */)
{
	QString qsFile = QFileDialog::getSaveFileName(this, tr("Private key file"), QString(), tr("private key (*.der)"));
	if (!qsFile.isEmpty()) {
		_qmwMainWindow.qlePrivate->setText(qsFile);
	} // if
} // on_qpbBrowsePrivate_clicked

const void MainWindow::on_qpbGenerate_clicked(bool checked /* false */) const
{
	CryptoPP::AutoSeededRandomPool asrpRandomPool;
	CryptoPP::InvertibleRSAFunction irfRsaFunc;
	irfRsaFunc.GenerateRandomWithKeySize(asrpRandomPool, KEY_SIZE);

	CryptoPP::RSAES_OAEP_SHA_Decryptor rosdDecryptor(asrpRandomPool, KEY_SIZE);
	CryptoPP::FileSink fsPrivateFile(_qmwMainWindow.qlePrivate->text().toLocal8Bit());
	rosdDecryptor.DEREncode(fsPrivateFile);
	fsPrivateFile.MessageEnd();

	CryptoPP::RSAES_OAEP_SHA_Encryptor roseEncryptor(rosdDecryptor);
	CryptoPP::FileSink fsPublicFile(_qmwMainWindow.qlePublic->text().toLocal8Bit());
	roseEncryptor.DEREncode(fsPublicFile);
	fsPublicFile.MessageEnd();
} // on_qpbGenerate_clicked