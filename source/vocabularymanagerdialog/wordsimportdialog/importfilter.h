#ifndef IMPORTFILTER_H
#define IMPORTFILTER_H

#include <QtCore/QString>

class ImportFilter
{
	public:
		ImportFilter(const QString &pFile);

		virtual const bool Open() const = 0;

	private:
		QString _qsFile;
}; // ImportFilter

#endif // IMPORTFILTER_H