#include "../../common/rsa.h"

#include "../../3rdparty/Crypto++/source/rsa.h"
#include "../../3rdparty/Crypto++/source/osrng.h"
#include <string>

QByteArray RSA::decrypt(const QByteArray &privateKey, const QByteArray &content) const
{
	CryptoPP::ArraySource keyBuffer(reinterpret_cast<const byte *>(privateKey.constData()), privateKey.size(), true);
	const CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(keyBuffer);

	CryptoPP::AutoSeededRandomPool randomPool;
	std::string decryptedString;
	const CryptoPP::ArraySource decryptedBuffer(reinterpret_cast<const byte *>(content.constData()), content.size(), true, new CryptoPP::PK_DecryptorFilter(randomPool, decryptor, new CryptoPP::StringSink(decryptedString)));

	return decryptedString.c_str();
} // decrypt

bool RSA::verify(const QByteArray &publicKey, const QByteArray &content, const QByteArray &signature) const
{
	CryptoPP::ArraySource keyBuffer(reinterpret_cast<const byte *>(publicKey.constData()), publicKey.size(), true);
	const CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::Verifier verifier(keyBuffer);

	if (signature.size() != verifier.SignatureLength())
	{
		return false;
	} // if

	CryptoPP::ArraySource signatureBuffer(reinterpret_cast<const byte *>(signature.constData()), signature.size(), true);
	CryptoPP::SecByteBlock secByteBlock(verifier.SignatureLength());
	signatureBuffer.Get(secByteBlock, secByteBlock.size());

	CryptoPP::VerifierFilter *verifierFilter = new CryptoPP::VerifierFilter(verifier);
	verifierFilter->Put(secByteBlock, verifier.SignatureLength());
	const CryptoPP::ArraySource contentBuffer(reinterpret_cast<const byte *>(content.constData()), content.size(), true, verifierFilter);

	return verifierFilter->GetLastResult();
} // verify