#include "plaintextimportwidget/plaintextfile.h"

const bool PlaintextFile::AtEnd() const
{
	return _qtsFile.atEnd();
} // AtEnd

const void PlaintextFile::Close()
{
	_qfFile.close();
} // Close

const QString PlaintextFile::GetCodecName() const
{
	return _qtcCodec->name();
} // GetCodecName

const bool PlaintextFile::IsOpen() const
{
	return _qfFile.isOpen();
} // IsOpen

const bool PlaintextFile::Open(const QString &pFile)
{
	_qfFile.setFileName(pFile);
	bool bOpen = _qfFile.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!bOpen) {
		return false;
	} // if

	OpenTextStream();
	return true;
} // Open

const void PlaintextFile::OpenTextStream(const QString *pCodec /* NULL */)
{
	QByteArray qbaContent = _qfFile.readAll();
	if (pCodec) {
		_qtcCodec = QTextCodec::codecForName(pCodec->toLocal8Bit());
	} else {
		_qtcCodec = QTextCodec::codecForUtfText(qbaContent);
	} // if else
	_qsCodecContent = _qtcCodec->toUnicode(qbaContent);
	_qtsFile.setString(&_qsCodecContent, QIODevice::ReadOnly | QIODevice::Text);
} // OpenTextStream

const QString PlaintextFile::ReadLine()
{
	return _qtsFile.readLine();
} // ReadLine

const void PlaintextFile::Seek(const qint64 &pPosition)
{
	_qtsFile.seek(pPosition);
} // Seek

const void PlaintextFile::SetCodecName(const QString &pCodec)
{
	_qfFile.seek(0);
	OpenTextStream(&pCodec);
} // SetCodecName