#ifndef PLAINTEXTFILE_H
#define PLAINTEXTFILE_H

#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

class PlaintextFile
{
	public:
		static const qint64 PFILE_BEGIN = 0;

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

		const void openTextStream(const QString *codec = nullptr);
}; // PlaintextFile

#endif // PLAINTEXTFILE_H