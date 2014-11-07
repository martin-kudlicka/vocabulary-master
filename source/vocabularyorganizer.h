#pragma once

#include "vocabulary.h"
#include "settings.h"

class VocabularyOrganizer : public QObject
{
	Q_OBJECT

	public:
		struct RecordInfo
		{
			Vocabulary *vocabulary;
			quint32     id;
		}; // RecordInfo
		struct VocabularyInfo
		{
			Settings::VocabularyInfo vocabularyInfo;
			Vocabulary              *vocabulary;
		}; // VocabularyInfo

		         VocabularyOrganizer(Settings *settings);
		virtual ~VocabularyOrganizer();

#ifndef EDITION_TRY
		      void            addExisting         (VocabularyInfo *vocabularyInfo, QWidget *parent);
#endif
		      void            addNew              (
#ifndef EDITION_TRY
			const QString &file
#endif
			);
		      bool            isOpen              ()             const;
#ifndef EDITION_TRY
		      void            openAll             (QWidget *parent);
#endif
			  quint32         recordCount         ()             const;
			  quint32         recordCount         (bool enabled) const;
			  RecordInfo      recordInfo          (quint32 row)  const;
		      void            remove              (quint8 index);
#ifndef EDITION_TRY
		      void            saveAll             ();
# ifndef EDITION_FREE
		      void            setVocabularyEnabled(quint8 index, bool enabled, QWidget *parent);
# endif
#endif
			  quint8          vocabularyCount     ()             const;
		const VocabularyInfo &vocabularyInfo      (quint8 index) const;

	private:
		QList<VocabularyInfo> _vocabularies;
		Settings             *_settings;

#ifndef EDITION_TRY
		void open(VocabularyInfo *vocabularyInfo, QWidget *parent);
#endif

	signals:
		void vocabularyClose(const Vocabulary *vocabulary) const;
}; // VocabularyOrganizer