#ifndef PLAINTEXTFILE_H
#define PLAINTEXTFILE_H

#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

class PlaintextFile
{
	public:
		static const qint64 PFILE_BEGIN = 0;

		const bool AtEnd() const;
		const void Close();
		const QString GetCodecName() const;
		const bool IsOpen() const;
		const bool Open(const QString &pFile);
		const QString ReadLine();
		const void Seek(const qint64 &pPosition);
		const void SetCodecName(const QString &pCodec);

	private:
		QFile _qfFile;
		QString _qsCodecContent;
		const QTextCodec *_qtcCodec;
		QTextStream _qtsFile;

		const void OpenTextStream(const QString *pCodec = NULL);
}; // PlaintextFile

#endif // PLAINTEXTFILE_H