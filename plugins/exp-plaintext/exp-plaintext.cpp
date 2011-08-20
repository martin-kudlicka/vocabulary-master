#include "exp-plaintext.h"

const QString ExpPlaintext::GetPluginName() const
{
	return tr("plaintext (txt)");
} // GetPluginName

const void ExpPlaintext::SetupUI(QGroupBox *pParent)
{

} // SetupUI

Q_EXPORT_PLUGIN2(exp-plaintext, ExpPlaintext)