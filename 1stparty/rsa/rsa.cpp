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
	CryptoPP::ArraySource asDecrypt(reinterpret_cast<const byte *>(pContent.constData()), pContent.size(), true, new CryptoPP::PK_DecryptorFilter(asrpRandomPool, rosdDecryptor, new CryptoPP::StringSink(sDecrypted)));

	return sDecrypted.c_str();
} // Decrypt

const QByteArray RSA::Encrypt(const QByteArray &pPublicKey, const QByteArray &pContent) const
{
	CryptoPP::ArraySource asKey(reinterpret_cast<const byte *>(pPublicKey.constData()), pPublicKey.size(), true);
	CryptoPP::RSAES_OAEP_SHA_Encryptor roseEncryptor(asKey);

	CryptoPP::AutoSeededRandomPool asrpRandomPool;
	std::string sEnrypted;
	CryptoPP::ArraySource asEncrypt(reinterpret_cast<const byte *>(pContent.constData()), pContent.size(), true, new CryptoPP::PK_EncryptorFilter(asrpRandomPool, roseEncryptor, new CryptoPP::StringSink(sEnrypted)));

	return QByteArray(sEnrypted.c_str(), sEnrypted.size());
} // Encrypt

const QByteArray RSA::Sign(const QByteArray &pPrivateKey, const QByteArray &pContent) const
{
	CryptoPP::ArraySource asKey(reinterpret_cast<const byte *>(pPrivateKey.constData()), pPrivateKey.size(), true);
	CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::Signer sSigner(asKey);

	CryptoPP::AutoSeededRandomPool asrpRandomPool;
	std::string sSigned;
	CryptoPP::ArraySource asSign(reinterpret_cast<const byte *>(pContent.constData()), pContent.size(), true, new CryptoPP::SignerFilter(asrpRandomPool, sSigner, new CryptoPP::StringSink(sSigned)));
	
	return QByteArray(sSigned.c_str(), sSigned.size());
} // Sign

const bool RSA::Verify(const QByteArray &pPublicKey, const QByteArray &pContent, const QByteArray &pSignature) const
{
	CryptoPP::ArraySource asKey(reinterpret_cast<const byte *>(pPublicKey.constData()), pPublicKey.size(), true);
	CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::Verifier vVerifier(asKey);

	if (pSignature.size() != vVerifier.SignatureLength()) {
		return false;
	} // if

	CryptoPP::ArraySource asSignature(reinterpret_cast<const byte *>(pSignature.constData()), pSignature.size(), true);
	CryptoPP::SecByteBlock sbbSecByteBlock(vVerifier.SignatureLength());
	asSignature.Get(sbbSecByteBlock, sbbSecByteBlock.size());

	CryptoPP::VerifierFilter *vfVerifierFilter = new CryptoPP::VerifierFilter(vVerifier);
	vfVerifierFilter->Put(sbbSecByteBlock, vVerifier.SignatureLength());
	CryptoPP::ArraySource asContent(reinterpret_cast<const byte *>(pContent.constData()), pContent.size(), true, vfVerifierFilter);

	return vfVerifierFilter->GetLastResult();
} // Verify