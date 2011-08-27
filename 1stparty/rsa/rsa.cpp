#include "../../common/rsa.h"

#include "../../3rdparty/Crypto++/source/rsa.h"
#include "../../3rdparty/Crypto++/source/osrng.h"
#include <string>

const QByteArray RSA::Decrypt(const QByteArray &pPrivateKey, const QByteArray &pContent) const
{
	CryptoPP::ArraySource asKey(reinterpret_cast<const byte *>(pPrivateKey.constData()), pPrivateKey.size(), true);
	CryptoPP::RSAES_OAEP_SHA_Decryptor rosdDecryptor(asKey);

	CryptoPP::AutoSeededRandomPool asrpRandomPool;
	std::string sDecrypted;
	CryptoPP::ArraySource(reinterpret_cast<const byte *>(pContent.constData()), pContent.size(), true, new CryptoPP::PK_DecryptorFilter(asrpRandomPool, rosdDecryptor, new CryptoPP::StringSink(sDecrypted)));

	return sDecrypted.c_str(); 
} // Decrypt

const QByteArray RSA::Encrypt(const QByteArray &pPublicKey, const QByteArray &pContent) const
{
	CryptoPP::ArraySource asKey(reinterpret_cast<const byte *>(pPublicKey.constData()), pPublicKey.size(), true);
	CryptoPP::RSAES_OAEP_SHA_Encryptor roseEncryptor(asKey);

	CryptoPP::AutoSeededRandomPool asrpRandomPool;
	std::string sEnrypted;
	CryptoPP::ArraySource(reinterpret_cast<const byte *>(pContent.constData()), pContent.size(), true, new CryptoPP::PK_EncryptorFilter(asrpRandomPool, roseEncryptor, new CryptoPP::StringSink(sEnrypted)));

	return sEnrypted.c_str();
} // Encrypt