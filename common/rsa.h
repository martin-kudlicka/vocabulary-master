#ifndef RSA_H
#define RSA_H

#include <QtCore/QByteArray>
#include "../../3rdparty/Crypto++/source/rsa.h"

class
# ifdef RSA_LIB
	Q_DECL_EXPORT
# else
	Q_DECL_IMPORT
# endif
	RSA
{
    public:
        const QByteArray Encrypt(const QString &pString) const;
        const void SetPrivateKey(const CryptoPP::RSA::PrivateKey &pKey);
        const void SetPublicKey(const CryptoPP::RSA::PublicKey &pKey);

    private:
        CryptoPP::RSA::PrivateKey _pkPrivateKey;
        CryptoPP::RSA::PublicKey _pkPublicKey;
}; // RSA

#endif // RSA_H