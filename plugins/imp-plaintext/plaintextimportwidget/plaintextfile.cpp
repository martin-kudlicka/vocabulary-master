#include "plaintextimportwidget/plaintextfile.h"

const bool PlaintextFile::atEnd() const
{
	return _textStream.atEnd();
} // atEnd

const void PlaintextFile::close()
{
	_file.close();
} // close

const QString PlaintextFile::codecName() const
{
	return _textCodec->name();
} // codecName

const bool PlaintextFile::isOpen() const
{
	return _file.isOpen();
} // isOpen

const bool PlaintextFile::open(const QString &fileName)
{
	_file.setFileName(fileName);
	const bool fileOpen = _file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!fileOpen)
	{
		return false;
	} // if

	openTextStream();
	return true;
} // open

const QString PlaintextFile::readLine()
{
	return _textStream.readLine();
} // readLine

const void PlaintextFile::seek(const qint64 &position)
{
	_textStream.seek(position);
} // seek

const void PlaintextFile::setCodecName(const QString &codec)
{
	_file.seek(0);
	openTextStream(&codec);
} // setCodecName

const void PlaintextFile::openTextStream(const QString *codec /* nullptr */)
{
	const QByteArray fileContent = _file.readAll();
	if (codec)
	{
		_textCodec = QTextCodec::codecForName(codec->toLocal8Bit());
	}
	else
	{
		_textCodec = QTextCodec::codecForUtfText(fileContent);
	} // if else
	_codecContent = _textCodec->toUnicode(fileContent);
	_textStream.setString(&_codecContent, QIODevice::ReadOnly | QIODevice::Text);
} // openTextStream