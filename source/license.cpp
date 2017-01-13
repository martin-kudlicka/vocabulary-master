#include "license.h"

#include "settings.h"
#include <QtCore/QString>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QFile>
#include "../common/rsa.h"

const auto ELEM_EMAIL     = "Email";
const auto ELEM_FIRSTNAME = "FirstName";
const auto ELEM_LASTNAME  = "LastName";
const auto ELEM_UID       = "UId";
const auto ELEM_VALIDTO   = "ValidTo";

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
  return _status == Status::Ok || _status == Status::Expired;
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
  const auto licenseData      = _settings->license();
  const int dataSize          = *reinterpret_cast<const int *>(licenseData.left(sizeof(dataSize)).constData());
  const auto encryptedContent = licenseData.mid(sizeof(dataSize), dataSize);
  const auto signature        = licenseData.mid(sizeof(dataSize) + dataSize);

  if (licenseData.isEmpty())
  {
    _status = Status::None;
    return;
  }

  // get sign key
  QFile signKeyFile(":/res/license/signpublic.der");
  signKeyFile.open(QIODevice::ReadOnly);
  const auto signKeyData = signKeyFile.readAll();

  // verify license
  const RSA rsa;
  auto verified = rsa.verify(signKeyData, encryptedContent, signature);
  if (!verified)
  {
    _status = Status::Invalid;
    return;
  }

  // get decrypt key
  QFile decryptKeyFile(":/res/license/encryptprivate.der");
  decryptKeyFile.open(QIODevice::ReadOnly);
  const auto decryptKeyData = decryptKeyFile.readAll();

  // decrypt license
  const auto decryptedContent = rsa.decrypt(decryptKeyData, encryptedContent);

  QXmlStreamReader xmlReader(decryptedContent);
  while (!xmlReader.atEnd())
  {
    const auto tokenType = xmlReader.readNext();
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
    _status = Status::Expired;
  }
  else
  {
    _status = Status::Ok;
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