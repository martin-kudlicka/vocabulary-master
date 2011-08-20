#ifndef EXP_PLAINTEXT_H
#define EXP_PLAINTEXT_H

#include "../common/exp-interface.h"

class ExpPlaintext : public QObject, private ExpInterface
{
	private:
		virtual const QString GetPluginName() const;
		virtual const void SetupUI(QGroupBox *pParent);
}; // ExpPlaintext

#endif // EXP_PLAINTEXT_H
