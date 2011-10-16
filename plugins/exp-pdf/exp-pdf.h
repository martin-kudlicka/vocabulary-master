#ifndef EXP_PDF_H
#define EXP_PDF_H

#include "../common/exp-interface.h"

class ExpPdf : public ExpInterface
{
	Q_OBJECT
	Q_INTERFACES(ExpInterface)

	private:
		virtual const bool BeginExport() const;
		virtual const QString GetPluginName() const;
		virtual const void SetupUI(QWidget *pParent);
}; // ExpPdf

#endif // EXP_PDF_H
