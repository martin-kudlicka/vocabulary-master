#include "license.h"

#include <QtCore/QString>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QFile>
#include "../common/rsa.h"

const QString ELEM_EMAIL     = "Email";
const QString ELEM_FIRSTNAME = "FirstName";
const QString ELEM_LASTNAME  = "LastName";
const QString ELEM_UID       = "UId";
const QString ELEM_VALIDTO   = "ValidTo";

License::License(const Settings *settings) : _settings(settings)
{
	refreshLicense();
}

const QString &License::email() const
{
	return _email;
}

const QString &License::firstName() const
{
    return _firstName;
}

bool License::isLoaded() const
{
	return _status == StatusOk || _status == StatusExpired;
}

/*bool License::isOk() const
{
	return _esStatus == StatusOk;
}*/

const QString &License::lastName() const
{
	return _lastName;
}

void License::refreshLicense()
{
	// split license into content and signature
	const QByteArray licenseData      = _settings->license();
	const qint16 dataSize             = *reinterpret_cast<const qint16 *>(licenseData.left(sizeof(dataSize)).constData());
	const QByteArray encryptedContent = licenseData.mid(sizeof(dataSize), dataSize);
	const QByteArray signature        = licenseData.mid(sizeof(dataSize) + dataSize);

	if (licenseData.isEmpty())
	{
		_status = StatusNone;
		return;
	}

	// get sign key
	QFile signKeyFile(":/res/license/signpublic.der");
	signKeyFile.open(QIODevice::ReadOnly);
	const QByteArray signKeyData = signKeyFile.readAll();

	// verify license
	const RSA rsa;
	bool verified = rsa.verify(signKeyData, encryptedContent, signature);
	if (!verified)
	{
		_status = StatusInvalid;
		return;
	}

	// get decrypt key
	QFile decryptKeyFile(":/res/license/encryptprivate.der");
	decryptKeyFile.open(QIODevice::ReadOnly);
	const QByteArray decryptKeyData = decryptKeyFile.readAll();

	// decrypt license
	const QByteArray decryptedContent = rsa.decrypt(decryptKeyData, encryptedContent);

	QXmlStreamReader xmlReader(decryptedContent);
	while (!xmlReader.atEnd())
	{
		const QXmlStreamReader::TokenType tokenType = xmlReader.readNext();
		if (tokenType != QXmlStreamReader::StartElement)
		{
			continue;
		}

		// general
		if (xmlReader.name() == ELEM_UID)
		{
			_identifier = xmlReader.readElementText();
			continue;
		}
		if (xmlReader.name() == ELEM_VALIDTO)
		{
			_validTo = QDate::fromString(xmlReader.readElementText(), Qt::ISODate);
			continue;
		}
		// personal
		if (xmlReader.name() == ELEM_FIRSTNAME)
		{
			_firstName = xmlReader.readElementText();
			continue;
		}
		if (xmlReader.name() == ELEM_LASTNAME)
		{
			_lastName = xmlReader.readElementText();
			continue;
		}
		if (xmlReader.name() == ELEM_EMAIL)
		{
			_email = xmlReader.readElementText();
			continue;
		}
	}

	if (QDate::currentDate() > _validTo)
	{
		_status = StatusExpired;
	}
	else
	{
		_status = StatusOk;
	}
}

const License::Status &License::status() const
{
	return _status;
}

const QUuid &License::uid() const
{
	return _identifier;
}

const QDate &License::validTo() const
{
	return _validTo;
}