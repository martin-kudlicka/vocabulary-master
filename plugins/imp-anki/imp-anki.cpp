#include "imp-anki.h"

const QString ImpAnki::GetFilter() const
{
	return "Anki (*.anki)";
} // GetFilter

const bool ImpAnki::Open(const QString &pFile) const
{
} // Open

Q_EXPORT_PLUGIN2(imp-anki, ImpAnki)