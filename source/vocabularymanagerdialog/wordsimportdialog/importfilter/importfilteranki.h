#ifndef IMPORTFILTERANKI_H
#define IMPORTFILTERANKI_H

#include "vocabularymanagerdialog/wordsimportdialog/importfilter.h"

class ImportFilterAnki : public ImportFilter
{
	public:
		ImportFilterAnki(const QString &pFile);

	private:
		const bool Open() const;
}; // ImportFilterAnki

#endif // IMPORTFILTERANKI_H