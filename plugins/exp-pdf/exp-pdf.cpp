#include "exp-pdf.h"

const bool ExpPdf::BeginExport() const
{
	// TODO
	return true;
} // BeginExport

const QString ExpPdf::GetPluginName() const
{
	return tr("pdf (pdf)");
} // GetPluginName

const void ExpPdf::SetupUI(QWidget *pParent)
{
	// TODO
} // SetupUI

Q_EXPORT_PLUGIN2(exp-pdf, ExpPdf)