#include "exp-plaintext.h"

#include "plaintextexportwidget.h"

const QString ExpPlaintext::GetPluginName() const
{
	return tr("plaintext (txt)");
} // GetPluginName

const void ExpPlaintext::SetupUI(QWidget *pParent)
{
	PlaintextExportWidget *pewWidget = new PlaintextExportWidget(pParent);
	QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
	pLayout->insertWidget(WIDGET_POSITION, pewWidget);
} // SetupUI

Q_EXPORT_PLUGIN2(exp-plaintext, ExpPlaintext)