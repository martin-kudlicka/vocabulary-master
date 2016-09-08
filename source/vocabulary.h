#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "vocabulary/vocabularydatabase.h"

class Settings;

class Vocabulary : public VocabularyDatabase
{
  public:
             Vocabulary(const Settings *settings);
    virtual ~Vocabulary() override;

    quintptr        addCategory           (const QString &name);
#ifndef EDITION_FREE
    void            addField              ();
#endif
    void            addRecord             (quintptr categoryId);
#ifndef EDITION_FREE
    void            addRecord             (quintptr categoryId, const QStringList &data);
#endif
    void            beginEdit             ();
    void            close                 ();
    void            endEdit               (bool save = true);
    bool            fieldHasAttribute     (quintptr fieldId, FieldAttribute attribute)          const;
    CategoryIdList  categoryIds           ()                                                    const;
    QString         dataText              (quintptr categoryId, quintptr row, quintptr fieldId) const;
    QString         dataText              (quintptr recordId, quintptr fieldId)                 const;
    FieldAttributes fieldAttributes       (quintptr fieldId)                                    const;
#ifndef EDITION_FREE
    FieldBuiltIn    fieldBuiltIn          (quintptr fieldId)                                    const;
#endif
    quintptr        fieldCount            ()                                                    const;
    quintptr        fieldId               (quintptr position)                                   const;
    FieldIdList     fieldIds              ()                                                    const;
    FieldLanguage   fieldLanguage         (quintptr fieldId)                                    const;
    QString         fieldName             (quintptr fieldId)                                    const;
    QString         fieldTemplateName     (quintptr fieldId)                                    const;
    FieldType       fieldType             (quintptr fieldId)                                    const;
#ifndef EDITION_FREE
    QStringList     record                (quintptr recordId)                                   const;
#endif
    quintptr        recordCategory        (quintptr recordId)                                   const;
    quintptr        recordCount           ()                                                    const;
    quintptr        recordCount           (quintptr categoryId)                                 const;
#ifndef EDITION_FREE
    quintptr        recordCount           (quintptr categoryId, bool enabled)                   const;
    quintptr        recordCount           (bool enabled)                                        const;
    bool            recordEnabled         (quintptr recordId)                                   const;
#endif
    quintptr        recordId              (quintptr row)                                        const;
    quintptr        recordId              (quintptr categoryId, quintptr row)                   const;
    RecordIdList    recordIds             (quintptr categoryId)                                 const;
    void            new2                  (
#ifndef EDITION_TRY
      const QString &filePath
#endif
      );
#ifndef EDITION_TRY
    void            open                  (const QString &filePath);
#else
    void            openMemory            ();
#endif
    void            removeCategory        (quintptr categoryId);
#ifndef EDITION_FREE
    void            removeField           (quintptr fieldId);
#endif
    void            removeRecord          (quintptr categoryId, quintptr row);
    void            setDataText           (quintptr categoryId, quintptr row, quintptr fieldId, const QString &data);
    void            setDataText           (quintptr recordId, quintptr fieldId, const QString &data);
#ifndef EDITION_FREE
    void            setFieldAttributes    (quintptr fieldId, FieldAttributes attributes);
    void            setFieldLanguage      (quintptr fieldId, FieldLanguage language);
    void            setFieldName          (quintptr fieldId, const QString &name);
    void            setFieldTemplateName  (quintptr fieldId, const QString &templateName);
    void            setRecordByRowCategory(quintptr oldCategoryId, quintptr recordRow, quintptr newCategoryId);
    void            swapFields            (quintptr sourceId, quintptr destinationId);
#endif

  private:
    struct FieldData
    {
      QString         templateName;
      QString         name;
      FieldType       type;
      FieldAttributes attributes;
#ifndef EDITION_FREE
      FieldBuiltIn    builtIn;
#endif
      FieldLanguage   language;
    };

    typedef QHash<quintptr, RecordIdList> CategoryRecordsHash;
    typedef QHash<quintptr, FieldData>    FieldDataHash;

          bool                _cacheEnabled;
          CategoryRecordsHash _categoryRecords;
          FieldDataHash       _fieldData;
          FieldDataHash       _fieldDataBackup;
          RecordDataHash     *_recordData;
          RecordDataHash      _recordDataBackup;
    const Settings           *_settings;

    void      clearCache();
    FieldData fieldData (quintptr fieldId) const;
    void      initCache ();
};

#endif