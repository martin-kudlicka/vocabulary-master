#include "pch.h"
#include "imp-plaintext.h"

#include "plaintextimportwidget.h"

const auto    CACHED_NONE   = std::numeric_limits<quintptr>::max();
const QString TEMPLATE_MARK = "${%1}";

ImpPlaintext::ImpPlaintext() : _cachedRecord(CACHED_NONE), _widget(Q_NULLPTR)
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
  const auto regExp = _widget->regExp();
  QStringList markList;
  for (auto regExpChar = regExp.constBegin(); regExpChar != regExp.constEnd(); ++regExpChar)
  {
    if (*regExpChar == '(')
    {
      const auto mark = TEMPLATE_MARK.arg(QString::number(markList.count() + 1));
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
  const auto fileLines = _widget->lineCount();
  auto lines = fileLines / _widget->linesPerRecord();
  if (fileLines % _widget->linesPerRecord())
  {
    ++lines;
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
      decltype(record) line = 0;
      _plaintextFile.seek(PlaintextFile::PFILE_BEGIN);
      while (line != record)
      {
        for (decltype(_widget->linesPerRecord()) lineIndex = 0; lineIndex < _widget->linesPerRecord(); ++lineIndex)
        {
          _plaintextFile.readLine();
        }
        ++line;
      }
    }

    // read record from file
    QString line;
    for (decltype(_widget->linesPerRecord()) lineIndex = 0; lineIndex < _widget->linesPerRecord() && !_plaintextFile.atEnd(); ++lineIndex)
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
  const auto markList  = marks();
  const auto markIndex = markList.indexOf(mark);

  // get data
  if (_cachedCapture.count() > markIndex + 1)
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
  _widget        = new PlaintextImportWidget(&_plaintextFile, parent);
  auto boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
  boxLayout->insertWidget(WIDGET_POSITION, _widget);
}