#ifndef PLAINTEXTFILE_H
#define PLAINTEXTFILE_H

#include <QtCore/QFile>
#include <QtCore/QTextStream>

class QTextCodec;

class PlaintextFile
{
  public:
    static const auto PFILE_BEGIN = 0;

    const bool    atEnd       () const;
    const void    close       ();
    const QString codecName   () const;
    const bool    isOpen      () const;
    const bool    open        (const QString &fileName);
    const QString readLine    ();
    const void    seek        (const qint64 &position);
    const void    setCodecName(const QString &codec);

  private:
          QFile       _file;
          QString     _codecContent;
    const QTextCodec *_textCodec;
          QTextStream _textStream;

    void openTextStream(const QString *codec = Q_NULLPTR);
};

#endif