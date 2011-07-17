#include "vocabularymanagerdialog/wordsimportdialog/importfilter/importfilteranki.h"

ImportFilterAnki::ImportFilterAnki(const QString &pFile) : ImportFilter(pFile)
{
} // ImportFilterAnki

const bool ImportFilterAnki::Open() const
{
	return true;
} // Open