#ifndef RSA_H
#define RSA_H

#include <QtCore/QByteArray>

class
# ifdef RSA_LIB
	Q_DECL_EXPORT
# else
	Q_DECL_IMPORT
# endif
	RSA
{
    public:
		const QByteArray Decrypt(const QByteArray &pPrivateKey, const QByteArray &pContent) const;
		const bool Verify(const QByteArray &pPublicKey, const QByteArray &pContent, const QByteArray &pSignature) const;
}; // RSA

#endif // RSA_H