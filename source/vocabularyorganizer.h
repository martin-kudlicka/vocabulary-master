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

          void            addExisting         (VocabularyInfo *vocabularyInfo, QWidget *parent);
          void            addNew              (const QString &file);
          bool            isOpen              ()               const;
          void            openAll             (QWidget *parent);
          quintptr        recordCount         ()               const;
          quintptr        recordCount         (bool enabled)   const;
          RecordInfo      recordInfo          (quintptr row)   const;
          void            remove              (quintptr index);
          void            saveAll             ();
#ifndef EDITION_FREE
          void            setVocabularyEnabled(quintptr index, bool enabled, QWidget *parent);
#endif
          quintptr         vocabularyCount    ()               const;
    const VocabularyInfo &vocabularyInfo      (quintptr index) const;

  private:
    QList<VocabularyInfo> _vocabularies;
    Settings             *_settings;

    void open(VocabularyInfo *vocabularyInfo, QWidget *parent);

  Q_SIGNALS:
    void vocabularyClose(const Vocabulary *vocabulary) const;
};