#include "imp-plaintext.h"

#include <QtGui/QBoxLayout>

const void ImpPlaintext::Close()
{
    if (_pfPlaintext.IsOpen()) {
        _pfPlaintext.Close();
    } // if
} // Close

const QString ImpPlaintext::GetFilter() const
{
    return tr("plaintext (*.txt)");
} // GetFilter

const QStringList ImpPlaintext::GetMarks() const
{
    QString qsRegExp = _piwWidget->GetRegExp();
	QStringList qslMarks;
	for (QString::const_iterator ciChar = qsRegExp.constBegin(); ciChar != qsRegExp.constEnd(); ciChar++) {
		if (*ciChar == '(') {
			QString qsMark = TEMPLATE_MARK.arg(QString::number(qslMarks.size() + 1));
			qslMarks.append(qsMark);
		} // if
	} // for

	return qslMarks;
} // GetMarks

const int ImpPlaintext::GetRecordCount() const
{
	int iFileLines = _piwWidget->GetLineCount();
	int iLines = iFileLines / _piwWidget->GetLinesPerRecord();
	if (iFileLines % _piwWidget->GetLinesPerRecord()) {
		iLines++;
	} // if

	return iLines;
} // GetRecordCount

const QString ImpPlaintext::GetRecordData(const int &pRecord, const QString &pMark)
{
	if (_iCachedRecord != pRecord) {
		if (_iCachedRecord + 1 != pRecord) {
			// seek to record in file
			int iLine = 0;
			_pfPlaintext.Seek(PlaintextFile::FILE_BEGIN);
			while (iLine != pRecord) {
				for (int iI = 0; iI < _piwWidget->GetLinesPerRecord(); iI++) {
					_pfPlaintext.ReadLine();
				} // for
				iLine++;
			} // while
		} // if

		// read record from file
		QString qsLine;
		for (int iI = 0; iI < _piwWidget->GetLinesPerRecord() && !_pfPlaintext.AtEnd(); iI++) {
			if (!qsLine.isEmpty()) {
				qsLine += ' ';
			} // if
			qsLine += _pfPlaintext.ReadLine();
		} // for

		// get capture
		QRegExp qreRegExp(_piwWidget->GetRegExp());
		if (qreRegExp.indexIn(qsLine) != -1) {
			_qslCachedCapture = qreRegExp.capturedTexts();
		} else {
			_qslCachedCapture.clear();
		} // if else

		_iCachedRecord = pRecord;
	} // if

	// get mark ID
	QStringList qslMarks = GetMarks();
	int iIndex = qslMarks.indexOf(pMark);

	// get data
	if (_qslCachedCapture.size() > iIndex + 1) {
		return _qslCachedCapture.at(iIndex + 1);
	} else {
		return QString();
	} // if else
} // GetRecordData

const bool ImpPlaintext::Open(const QString &pFile)
{
    Close();
	_iCachedRecord = CACHED_NONE;
    return _pfPlaintext.Open(pFile);
} // Open

const void ImpPlaintext::SetupUI(QGroupBox *pParent)
{
    _piwWidget = new PlaintextImportWidget(&_pfPlaintext, pParent);
    QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
    pLayout->insertWidget(WIDGET_POSITION, _piwWidget);
} // SetupUI

Q_EXPORT_PLUGIN2(imp-plaintext, ImpPlaintext)