#include "imp-plaintext.h"

#include <QtWidgets/QBoxLayout>

const QString TEMPLATE_MARK = "${%1}";

const void ImpPlaintext::close()
{
    if (_plaintextFile.isOpen())
	{
        _plaintextFile.close();
    } // if
} // close

const QString ImpPlaintext::filter() const
{
    return tr("text file (*.txt)");
} // filter

const QStringList ImpPlaintext::marks() const
{
    const QString regExp = _widget->regExp();
	QStringList markList;
	for (QString::const_iterator regExpChar = regExp.constBegin(); regExpChar != regExp.constEnd(); regExpChar++)
	{
		if (*regExpChar == '(')
		{
			const QString mark = TEMPLATE_MARK.arg(QString::number(markList.size() + 1));
			markList.append(mark);
		} // if
	} // for

	return markList;
} // marks

const bool ImpPlaintext::open(const QString &fileName)
{
	close();
	_cachedRecord = CACHED_NONE;
	return _plaintextFile.open(fileName);
} // open

const quint16 ImpPlaintext::recordCount() const
{
	const quint16 fileLines = _widget->lineCount();
	quint16 lines           = fileLines / _widget->linesPerRecord();
	if (fileLines % _widget->linesPerRecord())
	{
		lines++;
	} // if

	return lines;
} // recordCount

const QString ImpPlaintext::recordData(const quint16 &record, const QString &mark)
{
	if (_cachedRecord != record)
	{
		if (_cachedRecord + 1 != record)
		{
			// seek to record in file
			quint16 line = 0;
			_plaintextFile.seek(PlaintextFile::PFILE_BEGIN);
			while (line != record)
			{
				for (quint16 lineIndex = 0; lineIndex < _widget->linesPerRecord(); lineIndex++)
				{
					_plaintextFile.readLine();
				} // for
				line++;
			} // while
		} // if

		// read record from file
		QString line;
		for (quint16 lineIndex = 0; lineIndex < _widget->linesPerRecord() && !_plaintextFile.atEnd(); lineIndex++)
		{
			if (!line.isEmpty())
			{
				line += ' ';
			} // if
			line += _plaintextFile.readLine();
		} // for

		// get capture
		QRegExp regExp(_widget->regExp());
		if (regExp.indexIn(line) != -1)
		{
			_cachedCapture = regExp.capturedTexts();
		}
		else
		{
			_cachedCapture.clear();
		} // if else

		_cachedRecord = record;
	} // if

	// get mark ID
	const QStringList markList = marks();
	const quint8 markIndex     = markList.indexOf(mark);

	// get data
	if (_cachedCapture.size() > markIndex + 1)
	{
		return _cachedCapture.at(markIndex + 1);
	}
	else
	{
		return QString();
	} // if else
} // recordData

const void ImpPlaintext::setupUI(QGroupBox *parent)
{
    _widget = new PlaintextImportWidget(&_plaintextFile, parent);
    QBoxLayout *boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
    boxLayout->insertWidget(WIDGET_POSITION, _widget);
} // setupUI