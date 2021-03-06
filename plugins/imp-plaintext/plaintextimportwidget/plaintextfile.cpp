#include "pch.h"
#include "plaintextimportwidget/plaintextfile.h"

PlaintextFile::PlaintextFile() : _textCodec(Q_NULLPTR)
{
}

const bool PlaintextFile::atEnd() const
{
  return _textStream.atEnd();
}

const void PlaintextFile::close()
{
  _file.close();
}

const QString PlaintextFile::codecName() const
{
  return _textCodec->name();
}

const bool PlaintextFile::isOpen() const
{
  return _file.isOpen();
}

const bool PlaintextFile::open(const QString &fileName)
{
  _file.setFileName(fileName);
  const auto fileOpen = _file.open(QIODevice::ReadOnly | QIODevice::Text);
  if (!fileOpen)
  {
    return false;
  }

  openTextStream();
  return true;
}

const QString PlaintextFile::readLine()
{
  return _textStream.readLine();
}

const void PlaintextFile::seek(const qint64 &position)
{
  _textStream.seek(position);
}

const void PlaintextFile::setCodecName(const QString &codec)
{
  _file.seek(0);
  openTextStream(&codec);
}

void PlaintextFile::openTextStream(const QString *codec /* Q_NULLPTR */)
{
  const auto fileContent = _file.readAll();
  if (codec)
  {
    _textCodec = QTextCodec::codecForName(codec->toLocal8Bit());
  }
  else
  {
    _textCodec = QTextCodec::codecForUtfText(fileContent);
  }
  _codecContent = _textCodec->toUnicode(fileContent);
  _textStream.setString(&_codecContent, QIODevice::ReadOnly | QIODevice::Text);
}