#include "../../common/rsa.h"

#include <QtCore/QString>
#include "../../3rdparty/Crypto++/source/osrng.h"
#include <string>

const QByteArray RSA::Encrypt(const QString &pString) const
{
	CryptoPP::AutoSeededRandomPool asrpRandomPool;
	CryptoPP::RSAES_OAEP_SHA_Encryptor roseEncryptor(_pkPrivateKey);
	std::string sEnrypted;

	CryptoPP::StringSource(pString.toLocal8Bit(), true, new CryptoPP::PK_EncryptorFilter(asrpRandomPool, roseEncryptor, new CryptoPP::StringSink(sEnrypted)));

	return sEnrypted.c_str();
    /*////////////////////////////////////////////////
    // Generate keys
    AutoSeededRandomPool rng;

    InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, 3072);

    RSA::PrivateKey privateKey(params);
    RSA::PublicKey publicKey(params);

    string plain="RSA Encryption", cipher, recovered;

    ////////////////////////////////////////////////
    // Encryption
    RSAES_OAEP_SHA_Encryptor e(publicKey);

    StringSource(plain, true,
    new PK_EncryptorFilter(rng, e,
    new StringSink(cipher)
    ) // PK_EncryptorFilter
    ); // StringSource

    ////////////////////////////////////////////////
    // Decryption
    RSAES_OAEP_SHA_Decryptor d(privateKey);

    StringSource(cipher, true,
    new PK_DecryptorFilter(rng, d,
    new StringSink(recovered)
    ) // PK_DecryptorFilter
    ); // StringSource

    cout << "Recovered plain text" << endl;*/
} // Encrypt

const void RSA::SetPrivateKey(const CryptoPP::RSA::PrivateKey &pKey)
{
    _pkPrivateKey = pKey;
} // SetPrivateKey

const void RSA::SetPublicKey(const CryptoPP::RSA::PublicKey &pKey)
{
    _pkPublicKey = pKey;
} // SetPublicKey