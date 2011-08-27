#include "license.h"

#include <QtCore/QString>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QFile>
#include "../common/rsa.h"

const QString ELEM_EMAIL = "Email";
const QString ELEM_FIRSTNAME = "FirstName";
const QString ELEM_LASTNAME = "LastName";
const QString ELEM_UID = "UId";
const QString ELEM_VALIDTO = "ValidTo";

const QString &License::GetEmail() const
{
	return _qsEmail;
} // GetEmail

const QString &License::GetFirstName() const
{
    return _qsFirstName;
} // GetFirstName

const QString &License::GetLastName() const
{
	return _qsLastName;
} // GetLastName

const License::eStatus &License::GetStatus() const
{
	return _esStatus;
} // GetStatus

const QUuid &License::GetUid() const
{
	return _quIdentifier;
} // GetUid

const QDate &License::GetValidTo() const
{
	return _qdValidTo;
} // GetValidTo

License::License(const Settings *pSettings)
{
	_sSettings = pSettings;

    RefreshLicense();
} // License

const void License::RefreshLicense()
{
	// split license into content and signature
	QByteArray qbaLicense = _sSettings->GetLicense();
	qint16 qi16Size = *reinterpret_cast<const qint16 *>(qbaLicense.left(sizeof(qi16Size)).constData());
	QByteArray qbaContent = qbaLicense.mid(sizeof(qi16Size), qi16Size);
	QByteArray qbaSignature = qbaLicense.mid(sizeof(qi16Size) + qi16Size);

	if (qbaLicense.isEmpty()) {
		_esStatus = StatusNone;
		return;
	} // if

	// get sign key
	QFile qfSignKey(":/MainWindow/res/license/signpublic.der");
	qfSignKey.open(QIODevice::ReadOnly);
	QByteArray qbaSignKey = qfSignKey.readAll();

	// verify license
	RSA rRSA;
	bool bVerify = rRSA.Verify(qbaSignKey, qbaContent, qbaSignature);
	if (!bVerify) {
		_esStatus = StatusInvalid;
		return;
	} // if

	// get decrypt key
	QFile qfDecryptKey(":/MainWindow/res/license/encryptprivate.der");
	qfDecryptKey.open(QIODevice::ReadOnly);
	QByteArray qbaDecryptKey = qfDecryptKey.readAll();

	// decrypt license
	QByteArray qbaDecrypted = rRSA.Decrypt(qbaDecryptKey, qbaContent);

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
		if (qxsrXmlReader.name() == ELEM_EMAIL) {
			_qsEmail = qxsrXmlReader.readElementText();
			continue;
		} // if
	} // while

	if (QDate::currentDate() > _qdValidTo) {
		_esStatus = StatusExpired;
	} else {
		_esStatus = StatusOk;
	} // if
} // RefreshLicense