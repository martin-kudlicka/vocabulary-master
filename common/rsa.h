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
		QByteArray decrypt(const QByteArray &privateKey, const QByteArray &content)                             const;
		bool       verify (const QByteArray &publicKey, const QByteArray &content, const QByteArray &signature) const;
}; // RSA

#endif // RSA_H