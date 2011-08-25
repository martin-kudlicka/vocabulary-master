#include "license.h"

#include <QtCore/QString>
#include <QtCore/QXmlStreamReader>

const QString ELEM_FIRSTNAME = "FirstName";
const QString ELEM_LASTNAME = "LastName";
const QString ELEM_UID = "UId";
const QString ELEM_VALIDTO = "ValidTo";

const QDate &License::GetValidTo() const
{
	return _qdValidTo;
} // GetValidTo

const QString &License::GetFirstName() const
{
    return _qsFirstName;
} // GetFirstName

const QString &License::GetLastName() const
{
	return _qsLastName;
} // GetLastName

const QUuid &License::GetUid() const
{
	return _quIdentifier;
} // GetUid

License::License(const QByteArray &pLicenseData)
{
    // parse license
    QXmlStreamReader qxsrXmlReader(pLicenseData);
    while (!qxsrXmlReader.atEnd()) {
        QXmlStreamReader::TokenType ttType = qxsrXmlReader.readNext();
		if (ttType != QXmlStreamReader::StartElement) {
			continue;
		} // if

		// general
		if (qxsrXmlReader.name() == ELEM_UID) {
			_quIdentifier = qxsrXmlReader.text().toString();
			continue;
		} // if
		if (qxsrXmlReader.name() == ELEM_VALIDTO) {
			_qdValidTo = QDate::fromString(qxsrXmlReader.text().toString());
			continue;
		} // if
		// personal
		if (qxsrXmlReader.name() == ELEM_FIRSTNAME) {
			_qsFirstName = qxsrXmlReader.text().toString();
			continue;
		} // if
		if (qxsrXmlReader.name() == ELEM_LASTNAME) {
			_qsLastName = qxsrXmlReader.text().toString();
			continue;
		} // if
    } // while
} // License