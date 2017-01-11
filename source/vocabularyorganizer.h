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
      quintptr     id;
    };
    struct VocabularyInfo
    {
      Settings::VocabularyInfo vocabularyInfo;
      Vocabulary              *vocabulary;
    };

             VocabularyOrganizer(Settings *settings);
    virtual ~VocabularyOrganizer() Q_DECL_OVERRIDE;

#ifndef EDITION_TRY
          void            addExisting(VocabularyInfo *vocabularyInfo, QWidget *parent);
#endif
          void            addNew(
#ifndef EDITION_TRY
                                 const QString &file
#endif
                                 );
          bool            isOpen()                       const;
#ifndef EDITION_TRY
          void            openAll(QWidget *parent);
#endif
          quintptr        recordCount()                  const;
          quintptr        recordCount(bool enabled)      const;
          RecordInfo      recordInfo(quintptr row)       const;
          void            remove(quintptr index);
#ifndef EDITION_TRY
          void            saveAll();
# ifndef EDITION_FREE
          void            setVocabularyEnabled(quintptr index, bool enabled, QWidget *parent);
# endif
#endif
          quintptr         vocabularyCount()             const;
    const VocabularyInfo &vocabularyInfo(quintptr index) const;

  private:
    QList<VocabularyInfo> _vocabularies;
    Settings             *_settings;

#ifndef EDITION_TRY
    void open(VocabularyInfo *vocabularyInfo, QWidget *parent);
#endif

  Q_SIGNALS:
    void vocabularyClose(const Vocabulary *vocabulary) const;
};