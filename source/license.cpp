#include "license.h"

#include <QtCore/QString>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QFile>
#include "../common/rsa.h"

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

License::License(const Settings *pSettings)
{
	_sSettings = pSettings;

    RefreshLicense();
} // License

const void License::RefreshLicense()
{
	// get RSA private key
	QFile qfPrivateKey(":/MainWindow/res/license/private.der");
	qfPrivateKey.open(QIODevice::ReadOnly);
	QByteArray qbaPrivateKey = qfPrivateKey.readAll();

	// decrypt license
	RSA rRSA;
	QByteArray qbaDecrypted = rRSA.Decrypt(qbaPrivateKey, _sSettings->GetLicense());

	QXmlStreamReader qxsrXmlReader(qbaDecrypted);
	while (!qxsrXmlReader.atEnd()) {
		QXmlStreamReader::TokenType ttType = qxsrXmlReader.readNext();
		if (ttType != QXmlStreamReader::StartElement) {
			continue;
		} // if

		// general
		if (qxsrXmlReader.name() == ELEM_UID) {
			_quIdentifier = qxsrXmlReader.readElementText();
			continue;
		} // if
		if (qxsrXmlReader.name() == ELEM_VALIDTO) {
			_qdValidTo = QDate::fromString(qxsrXmlReader.readElementText(), Qt::ISODate);
			continue;
		} // if
		// personal
		if (qxsrXmlReader.name() == ELEM_FIRSTNAME) {
			_qsFirstName = qxsrXmlReader.readElementText();
			continue;
		} // if
		if (qxsrXmlReader.name() == ELEM_LASTNAME) {
			_qsLastName = qxsrXmlReader.readElementText();
			continue;
		} // if
	} // while
} // RefreshLicense