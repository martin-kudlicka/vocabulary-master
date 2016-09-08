#include "imp-plaintext.h"

#include <QtWidgets/QGroupBox>
#include <QtWidgets/QBoxLayout>

const QString TEMPLATE_MARK = "${%1}";

ImpPlaintext::~ImpPlaintext()
{
}

void ImpPlaintext::close()
{
    if (_plaintextFile.isOpen())
	{
        _plaintextFile.close();
    }
}

QString ImpPlaintext::filter() const
{
    return tr("text file (*.txt)");
}

QStringList ImpPlaintext::marks() const
{
    const QString regExp = _widget->regExp();
	QStringList markList;
	for (QString::const_iterator regExpChar = regExp.constBegin(); regExpChar != regExp.constEnd(); regExpChar++)
	{
		if (*regExpChar == '(')
		{
			const QString mark = TEMPLATE_MARK.arg(QString::number(markList.size() + 1));
			markList.append(mark);
		}
	}

	return markList;
}

bool ImpPlaintext::open(const QString &fileName)
{
	close();
	_cachedRecord = CACHED_NONE;
	return _plaintextFile.open(fileName);
}

quintptr ImpPlaintext::recordCount() const
{
	const quint16 fileLines = _widget->lineCount();
	quint16 lines           = fileLines / _widget->linesPerRecord();
	if (fileLines % _widget->linesPerRecord())
	{
		lines++;
	}

	return lines;
}

QString ImpPlaintext::recordData(quintptr record, const QString &mark)
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
				}
				line++;
			}
		}

		// read record from file
		QString line;
		for (quint16 lineIndex = 0; lineIndex < _widget->linesPerRecord() && !_plaintextFile.atEnd(); lineIndex++)
		{
			if (!line.isEmpty())
			{
				line += ' ';
			}
			line += _plaintextFile.readLine();
		}

		// get capture
		QRegExp regExp(_widget->regExp());
		if (regExp.indexIn(line) != -1)
		{
			_cachedCapture = regExp.capturedTexts();
		}
		else
		{
			_cachedCapture.clear();
		}

		_cachedRecord = record;
	}

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
	}
}

void ImpPlaintext::setupUI(QGroupBox *parent)
{
    _widget = new PlaintextImportWidget(&_plaintextFile, parent);
    QBoxLayout *boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
    boxLayout->insertWidget(WIDGET_POSITION, _widget);
}